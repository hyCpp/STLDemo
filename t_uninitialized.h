#ifndef T_UNINITIALIZED
#define T_UNINITIALIZED
#include "t_construct.h"
#include "t_traits.h"
#include <string.h>
#include "t_algorithm.h"

using namespace STL;

// ---------------uninitialized_copy------------------

template<typename InputIterator, typename ForwardIterator>
ForwardIterator __uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, __true_type)
{
    return std::copy(first, last, result);
}

template<typename InputIterator, typename ForwardIterator>
ForwardIterator __uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, __false_type)
{
    ForwardIterator cur = result;
    for (; first != last; ++first, ++cur) {
        construct(&*cur, *first);
    }

    return cur;
}

template<typename InputIterator, typename ForwardIterator, typename T>
ForwardIterator __uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result, T*)
{
    typedef typename __type_traits<T>::is_POD_type is_Pod;
    return __uninitialized_copy_aux(first, last, result, is_Pod());
}

template<typename InputIterator, typename ForwardIterator>
ForwardIterator uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result)
{
    return __uninitialized_copy(first, last, result, value_type(result));
}

inline char* uninitialized_copy(const char* first, const char* last, char* result)
{
    memmove(result, first, last - first);
    return result + (last -first);
}

inline wchar_t* uninitialized_copy(const wchar_t *first, const wchar_t *last, wchar_t *result)
{
    memmove(result, first, sizeof(wchar_t) * (last - first));
    return result + (last - first);
}

// ---------------uninitialized_fill------------------

template<typename ForwardIterator, typename T>
inline void __uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& value, __true_type)
{
    std::fill(first, last, value);
}

template<typename ForwardIterator, typename T>
inline void __uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& value, __false_type)
{
    ForwardIterator cur = first;
    for (; cur != last; ++cur) {
        construct(&*cur, value);
    }
}

template<typename ForwardIterator, typename T, class T1>
inline void __uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& value, T1*)
{
    typedef typename __type_traits<T1>::is_POD_type is_Pod;
    __uninitialized_fill_aux(first, last, value, is_Pod());
}

template<typename ForwardIterator, typename T>
inline void uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& value)
{
    __uninitialized_fill(first, last, value, value_type(first));
}

// ---------------uninitialized_fill_n------------------

template<typename ForwardIterator, typename Size, typename T>
inline ForwardIterator __uninitialized_fill_n_aux(ForwardIterator first, Size n, const T& value, __true_type)
{
    return fill_n(first, n, value);
}

template<typename ForwardIterator, typename Size, typename T>
inline ForwardIterator __uninitialized_fill_n_aux(ForwardIterator first, Size n, const T& value, __false_type)
{
    ForwardIterator cur = first;
    for (; n > 0; --n, ++cur) {
        construct(&*cur, value);
    }
    return cur;
}

template<typename ForwardIterator, typename Size, typename T, typename T1>
ForwardIterator __uninitialized_fill_n(ForwardIterator first, Size n, const T& value, T1*)
{
    typedef typename __type_traits<T1>::is_POD_type is_Pod;
    return __uninitialized_fill_n_aux(first, n, value, is_Pod());
}

template<typename ForwardIterator, typename Size, typename T>
ForwardIterator uninitialized_fill_n(ForwardIterator first, Size n, const T& value)
{
    return __uninitialized_fill_n(first, n, value, value_type(first));
}

#endif // T_UNINITIALIZED

