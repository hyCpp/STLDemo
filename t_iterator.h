#ifndef T_ITERATOR
#define T_ITERATOR

#include <memory>

struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};

template<typename T, typename distance>
struct input_iterator
{
    typedef input_iterator_tag iterator_category;
    typedef T                  value_type;
    typedef distance           difference_type;
    typedef T*                 pointer;
    typedef T&                 reference;
};

struct output_iterator
{
    typedef output_iterator_tag   iterator_category;
    typedef void                  value_type;
    typedef void                  difference_type;
    typedef void                  pointer;
    typedef void                  reference;
};

template<typename T, typename distance>
struct forward_iterator
{
    typedef forward_iterator_tag iterator_category;
    typedef T                    value_type;
    typedef distance             difference_type;
    typedef T*                   pointer;
    typedef T&                   reference;
};

template<typename T, typename distance>
struct bidirectional_iterator
{
    typedef bidirectional_iterator_tag iterator_category;
    typedef T                          value_type;
    typedef distance                   difference_type;
    typedef T*                         pointer;
    typedef T&                         reference;
};

template<typename T, typename distance>
struct random_access_iterator
{
    typedef random_access_iterator_tag iterator_category;
    typedef T                          value_type;
    typedef distance                   difference_type;
    typedef T*                         pointer;
    typedef T&                         reference;
};

template<typename category, typename T, typename distance = std::ptrdiff_t, typename pointer = T*, typename reference = T&>
struct iterator
{
    typedef category        iterator_category;
    typedef T               value_type;
    typedef distance        difference_type;
    typedef pointer         pointer_type;
    typedef reference       reference_type;
};

template<typename I>
struct iterator_traits
{
    typedef typename I::iterator_category   iterator_category;
    typedef typename I::value_type          value_type;
    typedef typename I::difference_type     difference_type;
    typedef typename I::pointer             pointer;
    typedef typename I::reference           reference;
};

template<typename T>
struct iterator_traits<T*>
{
    typedef random_access_iterator_tag iterator_category;
    typedef T                          value_type;
    typedef std::ptrdiff_t             difference_type;
    typedef T*                         pointer;
    typedef T&                         reference;
};

template<typename T>
struct iterator_traits<const T*>
{
    typedef random_access_iterator_tag iterator_category;
    typedef T                          value_type;
    typedef std::ptrdiff_t             difference_type;
    typedef T*                         pointer;
    typedef T&                         reference;
};

// -----------get iterator type------------

template<typename Iterator>
inline typename iterator_traits<Iterator>::iterator_category
iterator_category(const Iterator&)
{
    typedef typename iterator_traits<Iterator>::iterator_category category;
    return category();
}

template<typename Iterator>
inline typename iterator_traits<Iterator>::difference_type*
distance_type(const Iterator&)
{
    return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
}

template<typename Iterator>
inline typename iterator_traits<Iterator>::value_type*
value_type(const Iterator&)
{
    return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
}

// -----------distance------------

template<typename inputIterator>
inline typename iterator_traits<inputIterator>::difference_type
__distance(inputIterator first, inputIterator last, input_iterator_tag)
{
    typename iterator_traits<inputIterator>::difference_type n = 0;
    while (first != last) {
        ++first;
        ++n;
    }

    return n;
}

template<typename RandomAccessIterator>
inline typename iterator_traits<RandomAccessIterator>::difference_type
__distance(RandomAccessIterator first, RandomAccessIterator last, random_access_iterator_tag)
{
    return last - first;
}

template<typename inputIterator>
inline typename iterator_traits<inputIterator>::difference_type
distance(inputIterator first, inputIterator last)
{
    typedef typename iterator_traits<inputIterator>::iterator_category category;
    return __distance(first, last, category());
}

// -----------advance------------

template <typename inputIterator, typename distance>
inline void __advance(inputIterator& i, distance n, input_iterator_tag)
{
    while (n--) {
        ++i;
    }
}

template <typename BidirectionalIterator, typename distance>
inline void __advance(BidirectionalIterator& i, distance n, bidirectional_iterator_tag)
{
    if (n >= 0) {
        while (n--) {
            ++i;
        }
    }
    else {
        while (n++) {
            --i;
        }
    }
}

template<typename RandomAccessIterator, typename distance>
inline void __advance(RandomAccessIterator& i, distance n, random_access_iterator_tag)
{
    i += n;
}

template<typename inputIterator, typename distance>
inline void advance(inputIterator& i, distance n)
{
    __advance(i, n, iterator_category(i));
}

#endif // T_ITERATOR

