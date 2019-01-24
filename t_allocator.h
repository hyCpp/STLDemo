#ifndef T_ALLOCATOR
#define T_ALLOCATOR
#include <new>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <climits>
#include <memory>
#include "t_algorithm.h"

#if 0
#   include <new>
#   define __THROW_BAD_ALLOC throw bad_alloc
#elif !defined(__THROW_BAD_ALLOC)
#   include <iostream>
#   define __THROW_BAD_ALLOC std::cerr<< "out of memory" <<std::endl; exit(1)
#endif

template<typename T>
inline T* allocate(ptrdiff_t size, T*)
{
    std::set_new_handler(0);
    T* tmp = static_cast<T*>(::operator new((size_t)(size * sizeof(T))));
    if (tmp == 0) {
        std::cout<<"out of memory"<<std::endl;
        exit(1);
    }
    return tmp;
}

template<typename T>
inline void deallocate(T* buffer)
{
    ::operator delete(buffer);
}

template<typename _Tp>
class allocator;

template<>
class allocator<void>
{
public:
    typedef size_t      size_type;
    typedef ptrdiff_t   difference_type;
    typedef void*       pointer;
    typedef const void* const_pointer;
    typedef void        value_type;

    template<typename _Tp1>
    struct rebind
    { typedef allocator<_Tp1> other; };
};

template<typename T>
class allocator {
public:
    typedef T           value_type;
    typedef T*          pointer;
    typedef T&          reference;
    typedef const T*    const_pointer;
    typedef const T&    const_reference;
    typedef size_t      size_type;
    typedef ptrdiff_t   difference_type;

    static pointer allocate(size_type n)
    {
        if (n > max_size()) {
            std::cout<<"error allocate"<<std::endl;
        }
        return ::allocate((difference_type)n, (pointer)0);
    }

    static void deallocate(pointer p)
    {
        ::deallocate(p);
    }

    static void deallocate(pointer p, size_type n)
    {
        if (0 != n) {
            ::deallocate(p);
        }
    }

    pointer address(reference r)
    {
        return (pointer)&r;
    }

    const_pointer const_address(const_reference r)
    {
        return (const_pointer)&r;
    }

    static size_type max_size()
    {
        return size_t(-1) / sizeof(T);
    }

    size_type init_page_size() {
        return STL::max(size_type(1), size_type(4096 / sizeof(T)));
    }

};

template<typename T, class Alloc>
class simple_alloc
{
public:
    static T* allocate(size_t n)
    {
        return 0 == n ? 0 : (T*)Alloc::allocate(n * sizeof(T));
    }

    static T* allocate(void)
    {
        return (T*)Alloc::allocate(sizeof(T));
    }

    static void deallocate(T* p)
    {
        Alloc::deallocate(p, sizeof(T));
    }

    static void deallocate(T* p, size_t n)
    {
        if (0 != n) {
            Alloc::deallocate(p, n * sizeof(T));
        }
    }
};

//--------------------first alloc------------------------
//---------------Large memory allocation-----------------

template <int inst>
class __malloc_alloc_template
{
private:
    static void *oom_malloc(size_t);
    static void *oom_realloc(void*, size_t);
    static void (* __malloc_alloc_oom_handler)();

public:
    static void* allocate(size_t n)
    {
        void *result = malloc(n);
        if (result == nullptr) result = oom_malloc(n);
        return result;
    }

    static void deallocate(void *p, size_t n)
    {
        free(p);
    }

    static void* reallocate(void* p, size_t, size_t new_sz)
    {
        void* result = realloc(p, new_sz);
        if (0 == result) result = oom_realloc(p, new_sz);
        return result;
    }

    static void (*set_malloc_handler(void(*f)())) ()
    {
        void (*old)() = __malloc_alloc_oom_handler;
        __malloc_alloc_oom_handler = f;
        return (old);
    }
};

template <int inst>
void (* __malloc_alloc_template<inst>::__malloc_alloc_oom_handler)() = 0;

template <int inst>
void * __malloc_alloc_template<inst>::oom_malloc(size_t n)
{
    void (* my_malloc_handler)();
    void *result;

    while (true) {
        my_malloc_handler = __malloc_alloc_oom_handler;
        if (0 == my_malloc_handler) {
            __THROW_BAD_ALLOC;
        }
        (*my_malloc_handler)();
        result = malloc(n);
        if (result) return (result);
    }
}

template <int inst>
void *__malloc_alloc_template<inst>::oom_realloc(void* p, size_t n)
{
    void (* my_malloc_handler)();
    void *result;
    while (true) {
        my_malloc_handler = __malloc_alloc_oom_handler;
        if (0 == my_malloc_handler) { __THROW_BAD_ALLOC;}
        (* my_malloc_handler)();
        result = realloc(p, n);
        if (result) return (result);
    }
}

typedef __malloc_alloc_template<0> malloc_alloc;


//-------------------sub-allocation(samll memory)-----------------
//if less than 128byte, Using memory pool management
// maintain 16 free-lists, 8,16,24,32...128

enum {__ALIGN = 8};
enum {__MAX_BTYES = 128};
enum {__NFREELISTS = __MAX_BTYES/__ALIGN}; // free-lists numbers

template<bool threads, int inst>
class __default_alloc_template
{
private:
    //相当于x+7 & 11111000(-8)只能是8的倍数
    static size_t ROUND_UP(size_t bytes)
    {
        return (((bytes) + __ALIGN - 1 ) & ~(__ALIGN - 1));
    }

    union obj
    {
        union obj* free_list_link;
        char client_data[1];
    };

    static obj* volatile free_list[__NFREELISTS]; //16

    //决定用哪个free-lists区域
    static size_t FREELIST_INDEX(size_t bytes) {
        return (((bytes) + __ALIGN - 1) / __ALIGN -1);
    }

    static void* refill(size_t n);

    // memory pool
    static char *chunk_alloc(size_t size, int &nobjs);

    static char *start_free;
    static char *end_free;
    static size_t heap_size;

public:
    static void* allocate(size_t n)
    {
        if (n > (size_t)__MAX_BTYES) return (malloc_alloc::allocate(n));

        obj* volatile *my_free_list;
        obj* result;
        my_free_list = free_list + FREELIST_INDEX(n);
        result = *my_free_list;
        if (result == nullptr)
        {
            void *r = refill(ROUND_UP(n));
            return r;
        }

        *my_free_list = result->free_list_link;
        return (result);
    }

    static void deallocate(void* p, size_t n)
    {
        obj *q = (obj*)p;
        obj * volatile *my_free_list;

        if (n > (size_t) __MAX_BTYES) {
            malloc_alloc::deallocate(p, n);
            return;
        }

        my_free_list = free_list + FREELIST_INDEX(n);
        q->free_list_link = *my_free_list;
        *my_free_list = q;
    }

    static void* reallocate(void *p, size_t old_sz, size_t new_sz)
    {

    }
};

template<bool threads, int inst>
char* __default_alloc_template<threads, inst>::start_free = 0;

template<bool threads, int inst>
char* __default_alloc_template<threads, inst>::end_free = 0;

template<bool threads, int inst>
size_t __default_alloc_template<threads, inst>::heap_size = 0;

template<bool threads, int inst>
typename __default_alloc_template<threads, inst>::obj* volatile
__default_alloc_template<threads, inst>::free_list[__NFREELISTS] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

template <bool threads, int inst>
void* __default_alloc_template<threads, inst>::refill(size_t n)
{
    int nobjs = 20;
    char* chunk = chunk_alloc(n, nobjs);
    obj* volatile *my_free_list;
    obj* result;
    obj* current_obj, *next_obj;

    if (1 == nobjs) return (chunk);

    my_free_list = free_list + FREELIST_INDEX(n);
    result = (obj*)chunk;
    *my_free_list = next_obj = (obj*)(chunk + n);
    // 将一下freelist节点串联起来
    for (int i = 1; ; ++i) {
        current_obj = next_obj;
        next_obj = (obj*)((char*)next_obj + n);
        if (nobjs - 1 == i) {
            current_obj->free_list_link = 0;
            break;
        }
        else {
            current_obj->free_list_link = next_obj;
        }
    }
    return (result);
}

template <bool threads, int inst>
char* __default_alloc_template<threads, inst>::chunk_alloc(size_t size, int& nobjs)
{
    char* result;
    size_t total_bytes = size * nobjs;
    size_t bytes_left = end_free - start_free;

    if (bytes_left >= total_bytes) {
        result = start_free;
        start_free += total_bytes;
        return (result);
    }
    else if (bytes_left >= size){
        nobjs = bytes_left/size;
        total_bytes = size * nobjs;
        result = start_free;
        start_free += total_bytes;
        return (result);
    }
    else {
        size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size >> 4);
        if (bytes_left > 0) {
            obj* volatile *my_free_list = free_list + FREELIST_INDEX(bytes_left);
            ((obj*)start_free)->free_list_link = *my_free_list;
            *my_free_list = (obj*)start_free;
        }

        start_free = (char*)malloc(bytes_to_get);
        if (0 == start_free) {
            obj* volatile *my_free_list, *p;
            for (int i = size; i <= __MAX_BTYES; i += __ALIGN) {
                my_free_list = free_list + FREELIST_INDEX(i);
                p = *my_free_list;
                if (0 != p) {
                    *my_free_list = p->free_list_link;
                    start_free = (char*)p;
                    end_free = start_free + i;
                    return (chunk_alloc(size, nobjs));
                }
            }
            end_free = 0;
            start_free = (char*)malloc_alloc::allocate(bytes_to_get);
        }
        heap_size += bytes_to_get;
        end_free = start_free + bytes_to_get;
        return (chunk_alloc(size, nobjs));
    }
}

typedef __default_alloc_template<false, 0> default_alloc;

#endif // T_ALLOCATOR

