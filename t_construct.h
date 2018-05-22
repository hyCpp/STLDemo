#ifndef T_CONSTRUCT
#define T_CONSTRUCT

#include <new>
#include "t_traits.h"
#include <t_iterator.h>

template<typename T, typename T2>
inline void construct(T* p, const T2& value)
{
    new (p) T(value);
}

template<typename T>
inline void destroy(T* pointer)
{
    pointer->~T();
}

template<typename ForwardIteartor>
inline void __destroy_aux(ForwardIteartor first, ForwardIteartor last, __false_type)
{
    for (; first < last; ++first) {
        destroy(&*first);
    }
}

template<typename ForwardIteartor>
inline void __destroy_aux(ForwardIteartor first, ForwardIteartor last, __true_type)
{

}


template<typename ForwardIteartor, typename T>
inline void __destroy(ForwardIteartor first, ForwardIteartor last, T*)
{
    typedef typename __type_traits<T>::has_trivial_destructor trivial_destructor;
    __destroy_aux(first, last, trivial_destructor());
}

template<typename ForwardIteartor>
inline void destroy(ForwardIteartor first, ForwardIteartor last)
{
    __destroy(first, last, value_type(first));
}

#endif // T_CONSTRUCT

