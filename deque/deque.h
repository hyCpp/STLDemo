#ifndef DEQUE
#define DEQUE

#include <string.h>
#include "t_allocator.h"
#include "t_construct.h"
#include "t_iterator.h"
#include "t_uninitialized.h"
namespace STL {


inline size_t __deque_buf_size(size_t n, size_t sz)
{
    return n != 0 ? n : (sz < 512 ? size_t(512 / sz) : size_t(1));
}

template <typename T,  typename Ref, typename Ptr, size_t bufSize>
struct __deque_iterator
{
    typedef __deque_iterator<T, T&, T*, bufSize>                iterator;
    typedef __deque_iterator<T, const T&, const T*, bufSize>    const_iterator;
    typedef random_access_iterator_tag                          iterator_category;
    typedef T               value_type;
    typedef Ptr             pointer;
    typedef Ref             reference;
    typedef size_t          size_type;
    typedef std::ptrdiff_t  difference_type;
    typedef T**             map_pointer;

    typedef __deque_iterator self;

    static size_t buffer_size()
    {
        return __deque_buf_size(bufSize, sizeof(T));
    }

    void set_node(map_pointer _node)
    {
        node = _node;
        first = *_node;
        last =  first + difference_type(buffer_size());
    }

    difference_type operator - (const self& x) const
    {
        return difference_type(buffer_size()) * (node - x.node - 1) + (cur - first) + (x.last - x.cur);
    }

    reference operator * ()
    {
        return *cur;
    }

    pointer operator -> ()
    {
        return &(operator*());
    }

    self& operator ++ ()
    {
        ++cur;
        if (cur == last) {
            set_node(node + 1);
            cur = first;
        }
        return *this;
    }

    self operator ++ (int)
    {
        self temp = *this;
        ++*this;
        return temp;
    }

    self& operator -- ()
    {
        --cur;
        if (cur == first) {
            set_node(node - 1);
            cur = last;
        }
        return *this;
    }

    self operator -- (int)
    {
        self temp = *this;
        --*this;
        return temp;
    }

    // 随机存取，跳跃n个距离
    self& operator += (difference_type n)
    {
        difference_type offset = n + (cur - first);
        if (offset >= 0 && offset < difference_type(buffer_size())) { // 跳跃n个距离后，在同一个缓冲区
            cur += n;
        }
        else {
            difference_type node_offset = offset > 0 ? offset / difference_type(buffer_size()) : -difference_type((-offset - 1) / buffer_size()) - 1; // 切换到正确缓冲区
            set_node(node + node_offset);
            cur = first + (offset - node_offset * difference_type(buffer_size()));
        }
        return *this;
    }

    self operator + (difference_type n) const
    {
        self temp = *this;
        return temp += n;
    }

    self& operator -= (difference_type n) const
    {
        return *this += -n;
    }

    reference operator [] (difference_type n) const
    {
        return *(*this + n);
    }

    bool operator == (const self& x) const
    {
        return cur == x.cur;
    }

    bool operator != (const self& x) const
    {
        return cur != x.cur;
    }

    bool operator < (const self& x) const
    {
        return (node == x.node) ? (cur < x.cur) : (node < x.node);
    }

    T* cur;
    T* first;
    T* last;
    map_pointer node;
};

template <typename T, typename Alloc = default_alloc, size_t bufSize = 0>
class deque
{
public:
    typedef T               value_type;
    typedef value_type*     pointer;
    typedef value_type&     reference;
    typedef size_t          size_type;
    typedef std::ptrdiff_t  difference_type;

    typedef __deque_iterator<T, T&, T*, bufSize>    iterator;

public:
    deque(int n, const value_type& value)
        : start()
        , finish()
        , map(0)
        , map_size(0)
    {
        fill_initialize(n, value);
    }

private:
    void fill_initialize(int n, const value_type& value)
    {
        create_map_and_nodes(n);
        map_pointer cur;
        try {
            for (cur = start.node; cur < finish.node; ++cur) {
                uninitialized_fill(*cur, *cur + iterator::buffer_size(), value);
                uninitialized_fill(finish.first, finish.cur, value);
            }
        }
        catch (...) {

        }
    }

    void create_map_and_nodes(size_type num)
    {
        size_type num_nodes = num / iterator::buffer_size() + 1; // 节点个数 = 元素个数 / 缓冲区可容纳个数 + 1
        map_size = max(initial_map_size(), num_nodes + 2); // 最少8个，最多是所需要个数+2(前后预留1个)
        map = map_allocator::allocate(map_size);
        map_pointer nstart = map + (map_size - num_nodes) / 2;
        map_pointer nfinish = nstart + num_nodes - 1;

        map_pointer cur;
        try {
            for (cur = nstart; cur <= nfinish; ++cur) {
                *cur = allocate_node();
            }
        }
        catch (...) {
        }

        start.set_node(nstart);
        finish.set_node(nfinish);
        start.cur = start.first;
        finish.cur = finish.first + num % iterator::buffer_size();
    }

    size_type initial_map_size()
    {
        return 8;
    }

    pointer allocate_node()
    {
        return data_allocator::allocate(__deque_buf_size(bufSize, sizeof(T)));
    }

    void deallocate_node(pointer p)
    {
        data_allocator::deallocate(p, __deque_buf_size(bufSize, sizeof(T)));
    }

    void reallocate_map(size_type n, bool flag)
    {
        size_type old_num = finish.node - start.node + 1;
        size_type new_num = old_num + n;
        map_pointer new_nstart;
        if (map_size > 2 * new_num) {
            new_nstart = map + (map_size - new_num) / 2 + (flag ? n : 0);
            if (new_nstart < start.node) {
                copy(start.node, finish.node + 1, new_nstart);
            }
            else {
                std::copy_backward(start.node, finish.node + 1, new_nstart + old_num);
            }
        }
        else {
            size_type new_map_size = map_size + max(map_size, n) + 2;
            map_pointer new_map = map_allocator::allocate(new_map_size);
            new_nstart = new_map + (new_map_size - new_num) / 2 + (flag ? n : 0);
            copy(start.node, finish.node + 1, new_nstart);
            map_allocator::deallocate(map, map_size);
            map = new_map;
            map_size = new_map_size;
        }

        start.set_node(new_nstart);
        finish.set_node(new_nstart + old_num - 1);
    }

    void push_back_aux(const value_type& value)
    {
        value_type temp = value;
        reserve_map_at_back(); // 符合某种条件时重新换一个map(原map空间不够大)
        *(finish.node + 1) = allocate_node();

        try {
            construct(finish.cur, temp);
            finish.set_node(finish.node + 1);
            finish.cur = finish.first;
        }
        catch (...) {
            deallocate_node(*(finish.node + 1));
        }
    }

    void push_front_aux(const value_type& value)
    {
        value_type temp = value;
        reserve_map_at_front(); // 符合某种条件时重新换一个map(原map空间不够大)
        *(start.node - 1) = allocate_node();

        try {
            start.set_node(start.node - 1);
            start.cur = start.last - 1;
            construct(start.cur, temp);
        }
        catch (...) {
            start.set_node(start.node + 1);
            start.cur = start.first;
            deallocate_node(*(start.node - 1));
            throw;
        }
    }

    void pop_back_aux()
    {
        deallocate_node(finish.first);
        finish.set_node(finish.node - 1);
        finish.cur = finish.last - 1;
        destroy(finish.cur);
    }

    void pop_front_aux()
    {
        destroy(start.cur);
        deallocate_node(start.first);
        start.set_node(start.node + 1);
        start.cur = start.first;
    }

    void reserve_map_at_back(size_type node = 1)
    {
        if (node + 1 > map_size - (finish.node - map)) {
            reallocate_map(node, false);
        }
    }

    void reserve_map_at_front(size_type node = 1)
    {
        if (node > start.node - map) {
            reallocate_map(node, true);
        }
    }

    iterator insert_aux(iterator pos, const value_type& value)
    {
        difference_type index = pos - start;
        value_type temp = value;
        if (index < (size() >> 1)) {
            push_front(front());
            iterator front1 = start;
            ++front1;
            iterator front2 = front1;
            ++front2;
            pos = start + index;
            iterator pos1 = pos;
            ++pos1;
            copy(front2, pos1, front1);
        }
        else {
            push_back(back());
            iterator back1 = finish;
            --back1;
            iterator back2 = back1;
            --back2;
            pos = start + index;
            std::copy_backward(pos, back2, back1);
        }
        *pos = temp;
        return pos;
    }

public:
    iterator begin()
    {
        return start;
    }

    iterator end()
    {
        return finish;
    }

    reference operator [] (size_type n)
    {
        return start[difference_type(n)];
    }

    reference front()
    {
        return *start;
    }

    reference back()
    {
        // 因为finish的没有重载操作符，需要赋值给iterator
        iterator temp = finish;
        --temp;
        return *temp;
    }

    size_type size() const
    {
        return finish - start;
    }

    size_type max_size() const
    {
        return size_type(-1);
    }

    bool empty() const
    {
        return finish == start;
    }

    void push_back(const value_type& value)
    {
        if (finish.cur != finish.last - 1) { // 空间够用直接构造
            construct(finish.cur, value);
            ++finish.cur;
        }
        else { // 以无空间或者只剩一个空间
            push_back_aux(value);
        }
    }

    void push_front(const value_type& value)
    {
        if (start.cur != start.first) {
            construct(start.cur - 1, value);
            --start.cur;
        }
        else {
            push_front_aux(value);
        }
    }

    void pop_back()
    {
        if (finish.cur != finish.last) {
            --finish.cur;
            destroy(finish.cur);
        }
        else {
            pop_back_aux();
        }
    }

    void pop_front()
    {
        if (start.cur != start.last - 1) {
            destroy(start.cur);
            ++start.cur;
        }
        else {
            pop_front_aux();
        }
    }

    void clear()
    {
        for (map_pointer node = start.node + 1; node < finish.node; ++node) { // 头尾以外的缓冲区
            destroy(*node, *node + iterator::buffer_size());
            deallocate_node(*node);
        }

        if (start.node != finish.node) {
            destroy(start.cur, start.last); //将头缓冲区的现有元素清除
            destroy(finish.first, finish.cur); // 将尾缓冲区的现有元素清除
            deallocate_node(finish.first); // 尾缓冲区析构，头保留
        }
        else {
            destroy(start.cur, finish.cur);
        }

        finish = start;
    }

    iterator erase(iterator pos)
    {
        iterator next = pos;
        ++next;
        difference_type index = pos - start;
        if (index < (size() >> 1)) {
            std::copy_backward(start, pos, next);
            pop_front();
        }
        else {
            copy(next, finish, pos);
            pop_back();
        }
        return start + index;
    }

    iterator erase(iterator first, iterator last)
    {
        if (first == start && last == finish)
        {
            clear();
            return finish;
        }
        else {

        }
    }

    iterator insert(iterator pos, const value_type& x)
    {
        if (pos.cur == start.cur) { // 如果是最前端
            push_front(x);
            return start;
        }
        else if (pos.cur == finish.cur) { // 如果是尾端
            push_back(x);
            iterator temp = finish;
            --temp;
            return temp;
        }
        else {
            return insert_aux(pos, x);
        }
    }

protected:
    typedef simple_alloc<value_type, Alloc>     data_allocator;
    typedef simple_alloc<pointer, Alloc>        map_allocator;
    typedef pointer*    map_pointer;

    iterator            start;
    iterator            finish;
    map_pointer         map;
    size_type           map_size;
};

}
#endif // DEQUE

