#ifndef T_ALGORITHM
#define T_ALGORITHM
#include <cstring>
#include "t_iterator.h"
#include "t_traits.h"
#include "stdio.h"
#include "heap.h"
#include <iostream>

namespace STL {

template<typename InputIterator, typename Func>
Func for_each(InputIterator first, InputIterator last, Func f)
{
    for (; first != last; ++first) {
        f(*first);
    }
    return f;
}

template<typename OutPutIterator, typename Size, typename T>
OutPutIterator fill_n(OutPutIterator first, Size n, const T& value)
{
    for (; n > 0; --n, ++first) {
        *first = value;
    }
    return first;
}

template<typename ForwardIterator, typename T>
void fill(ForwardIterator first, ForwardIterator last, const T& value)
{
    for (; first != last; ++first) {
        *first = value;
    }
}

template<typename InputIterator, typename InputIterator2>
inline bool equal(InputIterator first1, InputIterator last1, InputIterator2 first2)
{
    for (; first1 != last1; ++first1, ++first2) {
        if (*first1 != *first2) {
            return false;
        }
    }
    return true;
}

template<typename InputIterator, typename InputIterator2, typename BinaryPredicate>
inline bool equal(InputIterator first1, InputIterator last1, InputIterator2 first2, BinaryPredicate binary)
{
    for (; first1 != last1; ++first1, ++first2) {
        if (!binary(*first1, *first2)) {
            return false;
        }
    }
    return true;
}

template<typename InputIterator, typename T>
inline InputIterator find(InputIterator first, InputIterator last, const T& value)
{
    while (first != last && *first != value) {
        ++first;
    }
    return first;
}


template<typename T>
inline const T& max(const T& a, const T& b)
{
    return a < b ? b : a;
}

template<typename T, typename Compare>
inline const T& max(const T& a, const T& b, Compare comp)
{
    return comp(a, b) ? b : a;
}

template<typename T>
inline const T& min(const T& a, const T& b)
{
    return a > b ? b : a;
}

template<typename T, typename Compare>
inline const T& min(const T& a, const T& b, Compare comp)
{
    return comp(b, a) ? b : a;
}

template <typename T>
inline void swap(T& a, T&b)
{
    T tmp = a;
    a = b;
    b = tmp;
}

// --------------copy----------------

template<typename T>
inline T* __copy_t(const T* first, const T* last, T* result, __true_type)
{
    printf("__copy_d __true_type\n");
    memmove(result, first, sizeof(T) * (last - first));
    return result + (last - first);
}

template<typename T>
inline T* __copy_t(const T* first, const T* last, T* result, __false_type)
{
    printf("__copy_d __false_type\n");
    return __copy_d(first, last, result, (std::ptrdiff_t*)0);
}

template<typename RandomAccessIterator, typename OutputIterator, typename Distance>
inline OutputIterator __copy_d(RandomAccessIterator first, RandomAccessIterator last, OutputIterator result, Distance*)
{
    printf("__copy_d\n");
    for (Distance n = last - first; n > 0; --n, ++result, ++first) {
        *result = *first;
    }

    return result;
}

template<typename InputIterator, typename OutputIterator>
inline OutputIterator __copy(InputIterator first, InputIterator last, OutputIterator result, input_iterator_tag)
{
    printf("__copy input_iterator_tag \n");
    for (; first != last; ++first, ++result) {
        *result = *first;
    }

    return result;
}

template<typename RandomAccessIterator, typename OutputIterator>
inline OutputIterator __copy(RandomAccessIterator first, RandomAccessIterator last, OutputIterator result, random_access_iterator_tag)
{
    printf("__copy random_access_iterator_tag ");
    return __copy_d(first, last, result, distance_type(first));
}

template<typename InputIterator, typename OutputIterator>
struct __copy_dispatch
{
    OutputIterator operator()(InputIterator first, InputIterator last, OutputIterator result)
    {
        printf("__copy_dispatch ");
        return __copy(first, last, result, iterator_category(first));
    }
};

template<typename T>
struct __copy_dispatch<const T*, T*>
{
    T* operator()(const T* first, const T* last, T* result)
    {
        printf("__copy_dispatch<const T*, T*> ");
        typedef typename __type_traits<T>::has_trivial_assignment_operator t;
        return __copy_t(first, last, result, t());
    }
};

template<typename T>
struct __copy_dispatch<T*, T*>
{
    T* operator()(T* first, T* last, T* result)
    {
        printf("__copy_dispatch<T*, T*> ");
        typedef typename __type_traits<T>::has_trivial_assignment_operator t;
        return __copy_t(first, last, result, t());
    }
};

template<typename InputIterator, typename OutputIterator>
inline OutputIterator copy(InputIterator first, InputIterator last, OutputIterator result)
{
    return __copy_dispatch<InputIterator, OutputIterator>()(first, last, result);
}

inline char* copy(const char* first, const char* last, char* result)
{
    memmove(result, first, last - first);
    return result + (last- first);
}

inline wchar_t* copy(const wchar_t* first, const wchar_t* last, wchar_t* result)
{
    memmove(result, first, sizeof(wchar_t) * (last - first));
    return result + (last- first);
}

// --------------iter_swap----------------

template<typename ForwardIterator1, typename ForwardIterator2, typename T>
inline void __iter_swap(ForwardIterator1 a, ForwardIterator2 b, T*)
{
    T tmp = *a;
    *a = *b;
    *b = tmp;
}

template <typename ForwardIterator1, typename ForwardIterator2>
inline void iter_swap(ForwardIterator1 a, ForwardIterator2 b)
{
    __iter_swap(a, b, value_type(a));
}

// --------------sort----------------

template<typename RandomAccessIterator, typename T>
void __unguarded_linear_insert(RandomAccessIterator last, T value)
{
    RandomAccessIterator next = last;
    --next;
    while (value < * next) {
        *last = *next;
        last = next;
        --next;
    }
    *last = value;
}

template<typename RandomAccessIterator, typename T>
inline void __linear_insert(RandomAccessIterator first, RandomAccessIterator last, T*)
{
    T value = *last;
    if (value < *first) {
        std::copy_backward(first, last, last + 1);
        *first = value;
    }
    else {
        __unguarded_linear_insert(last, value);
    }
}

template<typename RandomAccessIterator>
void __insertion_sort(RandomAccessIterator first, RandomAccessIterator last)
{
    if (first == last) return;
    for (RandomAccessIterator i = first + 1; i != last; ++i) {
        __linear_insert(first, i, value_type(first));
    }
}

template <typename Size>
inline Size __lg(Size n) // 2 ^ k <= n
{
    Size k;
    for (k = 0; n > 1; n >>= 1) {  // n = n / 2
        ++k;
    }
    return k;
}

template <typename T>
inline const T& __median(const T& a, const T& b, const T& c)
{
    if (a < b) {
        if (b < c) { // a < b < c
            return b;
        }
        else if (a < c) { // a < c < b
            return c;
        }
        else {
            return a; // c < a < b
        }
    }
    else if (a < c) { // b <= a < c
        return a;
    }
    else if (b < c) { // c <= a ,b <= a, b < c
        return c;
    }
    else {
        return b;
    }
}

template <typename RandomAccessIterator, typename T>
RandomAccessIterator __unguarded_partition(RandomAccessIterator first, RandomAccessIterator last, T pivot)
{
    while (true) {
        while (*first < pivot) ++first;
        --last;
        while (pivot < *last) --last;

        if (!(first < last)) {
            return first;
        }
        iter_swap(first, last);
        ++first;
    }
}

template <typename RandomAccessIterator, typename T>
void __partial_sort(RandomAccessIterator first, RandomAccessIterator middle, RandomAccessIterator last, T*)
{
    make_heap(first, middle);
    for (RandomAccessIterator i = middle; i < last; ++i) {
        if (*i < *first) {
            __pop_heap(first, middle, i, T(*i), distance_type(first));
        }
    }

    sort_heap(first, middle);
}

template <typename RandomAccessIterator>
inline void partial_sort(RandomAccessIterator first, RandomAccessIterator middle, RandomAccessIterator last)
{
    __partial_sort(first, middle, last);
}

template<typename T>
struct display
{
    void operator () (const T& x) const
    {
        std::cout<<x<<", ";
    }
};

template <typename RandomAccessIterator, typename T, typename Size>
inline void __introSort_loop(RandomAccessIterator first , RandomAccessIterator last, T*, Size depth_limit) // RandomAccessIterator是指针类型,该函数不会修改指针,会修改指向内容,将序列分割成多个有序片段
{

    while (last - first > 16) { // 先在loop内分割直到分割恶化
        if (depth_limit == 0) { // 分割恶化进行heap sort
//            partial_sort(first, last, last);
            std::cout<<"return"<<std::endl;
            return;
        }
        std::cout<<"before depth_limit = "<<depth_limit<<", first = "<<*first<<", last = "<<*(last-1)<<" middle = "<<T(__median(*first, *(first + (last - first) / 2), *(last - 1)))<<std::endl;

        --depth_limit;
        RandomAccessIterator cut = __unguarded_partition(first, last, T(__median(*first, *(first + (last - first) / 2), *(last - 1)))); // 找到中值, 以中值进行分割，返回中值右边区域

        STL::for_each(cut, last, display<int>());

        std::cout<<"cut = "<<*cut<<std::endl;
        __introSort_loop(cut, last, value_type(first), depth_limit); // 分割好最左边的序列, 直到分割恶化的次数, if depth_limit = 10, execute partition 10 times
        last = cut; // 将最左边序列的first赋予为last, loop, 继续分割

        std::cout<<"after depth_limit = "<<depth_limit<<", first = "<<*first<<", last = "<<*(last-1)<<" middle = "<<T(__median(*first, *(first + (last - first) / 2), *(last - 1)))<<std::endl;
        STL::for_each(first, last, display<int>());
    }
}

template<typename RandomAccessIterator, typename T>
void __unguarded_insertion_sort_aux(RandomAccessIterator first, RandomAccessIterator last, T*)
{
    for (RandomAccessIterator i = first; i != last; ++i) {
        __unguarded_linear_insert(i, T(*i));
    }
}

template<typename RandomAccessIterator>
inline void __unguarded_insertion_sort(RandomAccessIterator first, RandomAccessIterator last)
{
    __unguarded_insertion_sort_aux(first, last, value_type(first));
}

template <typename RandomAccessIterator>
inline void __final_insertion_sort(RandomAccessIterator first, RandomAccessIterator last)
{
    std::cout<<"__final_insertion_sort"<<std::endl;
    if (last -first > 16) {
        __insertion_sort(first, first + 16);
        __unguarded_insertion_sort(first + 16, last);
    }
    else {
        __insertion_sort(first, last);
    }
}

template<typename RandomAccessIterator>
inline void sort(RandomAccessIterator first, RandomAccessIterator last)
{
    if (first != last) {
        std::cout<<"sort begin"<<std::endl;
        __introSort_loop(first, last, value_type(first), __lg(last - first) * 2);
        std::cout<<std::endl;
        STL::for_each(first, last, display<int>());
        __final_insertion_sort(first, last);
    }
}


}

#endif // T_ALGORITHM

