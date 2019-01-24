#ifndef HEAP
#define HEAP
#include <t_iterator.h>
#include <iostream>
namespace STL{

//     ====================push heap======================

template <typename RandomAccessIterator, typename Distance, typename T>
inline void __push_heap(RandomAccessIterator first, Distance holeIndex, Distance topIndex, T value)
{
    std::cout<<holeIndex<<std::endl;
    // 根节点：i ，左子节点：2i 右子节点：2i+1, 子节点小于等于父节点
    Distance parent = (holeIndex - 1) / 2; // The parent node of value, Index from 0

    while (holeIndex > topIndex && *(first + parent) < value) {
        *(first + holeIndex) = *(first + parent);
        holeIndex = parent;
        parent = (holeIndex - 1) / 2;
    }
    *(first + holeIndex) = value;
}

template <typename RandomAccessIterator, typename Distance, typename T>
inline void __push_heap_aux(RandomAccessIterator first, RandomAccessIterator last, Distance*, T*)
{
    __push_heap(first, Distance((last - first) - 1), Distance(0), T(*(last - 1)));
}

// 调用push_heap时，插入元素已经位于容器低端
template <typename RandomAccessIterator>
inline void push_heap(RandomAccessIterator first, RandomAccessIterator last)
{
    __push_heap_aux(first, last, distance_type(first), value_type(first));
}


//    ====================pop heap======================

template <typename RandomAccessIterator, typename Distance, typename T>
void __adjust_heap(RandomAccessIterator first, Distance holeIndex, Distance len, T value)
{
    Distance topIndex = holeIndex;
    Distance secondChild = 2 * holeIndex + 2; // The right child node of root
    while (secondChild < len) {
        if (*(first + secondChild) < *(first + (secondChild - 1))) {
            secondChild--;
        }
        *(first + holeIndex) = *(first + secondChild);
        holeIndex = secondChild;
        secondChild = 2 * (secondChild + 1);
    }

    if (secondChild == len) {
        *(first + holeIndex) = *(first + (secondChild - 1));
        holeIndex = secondChild - 1;
    }
    __push_heap(first, holeIndex, topIndex, value);
}

template <typename RandomAccessIterator, typename Distance, typename T>
inline void __pop_heap(RandomAccessIterator first, RandomAccessIterator last, RandomAccessIterator result, T value, Distance*)
{
    *result = *first;
    __adjust_heap(first, Distance(0), Distance(last - first), value);
}

template <typename RandomAccessIterator, typename T>
inline void __pop_heap_aux(RandomAccessIterator first, RandomAccessIterator last, T *)
{
    __pop_heap(first, last - 1, last - 1, T(*(last - 1)), distance_type(first));
}

template <typename RandomAccessIterator>
inline void pop_heap(RandomAccessIterator first, RandomAccessIterator last)
{
    __pop_heap_aux(first, last, value_type(first));
}


//    ====================sort heap======================

template <typename RandomAccessIterator>
void sort_heap(RandomAccessIterator first, RandomAccessIterator last)
{
    while (last - first > 1) {
        pop_heap(first, last--);
    }
}



//    ====================make heap======================

template <typename RandomAccessIterator, typename T, typename Distance>
void __make_heap(RandomAccessIterator first, RandomAccessIterator last, T*, Distance*)
{
    if (last - first < 2) {
        return;
    }

    Distance len = last - first;
    Distance parent = (len - 2) / 2;
    while (true) {
        __adjust_heap(first, parent, len, T(*(first + parent)));
        if (parent == 0) return;
        parent--;
    }
}

template <typename RandomAccessIterator>
inline void make_heap(RandomAccessIterator first, RandomAccessIterator last)
{
    __make_heap(first, last, value_type(first), distance_type(first));
}

}

#endif // HEAP

