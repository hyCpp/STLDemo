#ifndef LIST_
#define LIST_

#include <iostream>
#include <stddef.h>
#include <string.h>
#include "t_allocator.h"
#include "t_construct.h"
#include "t_iterator.h"

namespace STL {


template<typename T>
struct __list_node
{
    typedef void* void_pointer;
    void_pointer prev;
    void_pointer next;
    T data;
};

template<typename T>
struct __list_iter : public iterator<forward_iterator_tag, T>
{
    typedef __list_iter<T>      iterator;
    typedef __list_iter<T>      self;

    typedef bidirectional_iterator_tag          iterator_category;
    typedef T                                   value_type;
    typedef T*                                  pointer;
    typedef T&                                  reference;
    typedef __list_node<T>*                     link_type;
    typedef size_t                              size_type;
    typedef std::ptrdiff_t                      difference_type;

    link_type node;

    // constructor
    __list_iter(link_type p)
        : node(p)
    {
    }

    __list_iter()
        : node()
    {
    }

    __list_iter(const iterator& i)
        : node(i.node)
    {
    }

    // operator overload
    bool operator == (const self& i) const
    {
        return node == i.node;
    }

    bool operator != (const self& i) const
    {
        return node != i.node;
    }

    reference operator * () const
    {
        return (*node).data;
    }

    pointer operator -> () const
    {
        return &(operator*());
    }

    self& operator ++ ()
    {
        node = static_cast<link_type>((*node).next);
        return *this;
    }

    self operator ++ (int)
    {
        self tmp = *this;
        ++*this;
        return tmp;
    }

    self& operator -- ()
    {
        node = static_cast<link_type>((*node).prev);
        return *this;
    }

    self operator -- (int)
    {
        self tmp = *this;
        --*this;
        return tmp;
    }
};

template<typename T, typename Alloc = default_alloc>
class list
{
private:
    typedef __list_node<T>          list_node;
    typedef simple_alloc<list_node, Alloc>  list_allocator;

public:
    typedef __list_iter<T>          iterator;
    typedef const __list_iter<T>    const_iterator;
    typedef size_t                  size_type;
    typedef T                       value_type;
    typedef std::ptrdiff_t          difference_type;
    typedef T&                      reference;
    typedef T*                      pointer;

    typedef list_node* link_type;

    list()
    {
        empty_initialize();
    }

    list(const list& l)
    {
        empty_initialize();

        for (iterator i = l.begin(); i != l.end(); ++i) {
            push_back(*i);
        }
    }

    list& operator = (const list& __x);

    list(size_type n, const value_type& value)
    {
        empty_initialize();
        for(int i = 0; i < n; ++i) {
            push_back(value);
        }
    }

    template<typename _InputIterator>
    list(_InputIterator begin, _InputIterator end)
    {
        empty_initialize();
        for (; begin != end; ++begin) {
            push_back(*begin);
        }
    }

    iterator begin()
    {
        return static_cast<link_type>((*node).next);
    }

    iterator end()
    {
        return node;
    }

    const_iterator begin() const
    {
        return static_cast<link_type>((*node).next);
    }

    const_iterator end() const
    {
        return node;
    }

    bool empty() const
    {
        return node->next == node;
    }

    size_type size() const
    {
        size_type result = 0;
        distance(begin(), end(), result);
        return result;
    }

    reference front()
    {
        return *begin();
    }

    reference back()
    {
        return *(--end());
    }

    iterator insert(iterator pos, const T& t)
    {
        link_type tmp = this->create_node(t);
        tmp->next = pos.node;
        tmp->prev = pos.node->prev;
        (link_type(pos.node->prev))->next = tmp;
        pos.node->prev = tmp;
        return tmp;
    }

    iterator erase(iterator pos)
    {
        link_type next_node = link_type(pos.node->next);
        link_type prev_node = link_type(pos.node->prev);
        prev_node->next = next_node;
        next_node->prev = prev_node;
        destroy_node(pos.node);
        return iterator(next_node);
    }

    void push_front(const T& t)
    {
        insert(begin(), t);
    }

    void push_back(const T& t)
    {
        insert(end(), t);
    }

    void pop_front()
    {
        erase(begin());
    }

    void pop_back()
    {
        iterator tmp = end();
        erase(--tmp);
    }

    void clear()
    {
        link_type cur = (link_type)node->next;
        while (cur != node) {
            link_type tmp = cur;
            cur = (link_type)cur->next;
            destroy_node(tmp);
        }

        node->next = node;
        node->prev = node;
    }

    void remove(const T& value)
    {
        for (iterator i = begin(); i != end(); ++i) {
            if (*i == value) {
                erase(i);
            }
        }
    }

    void splice(iterator pos, list& x)
    {
        if (!x.empty()) {
            transfer(pos, x.begin(), x.end());
        }
    }

    void splice(iterator pos, list&, iterator i)
    {
        iterator j = i;
        ++j;
        if (pos == i || pos ==j) {
            return;
        }
        transfer(pos, i , j);
    }

    void splice(iterator pos, list &, iterator first, iterator last)
    {
        if (first != last) {
            transfer(pos, first, last);
        }
    }

    void reverse()
    {
        if (node->next == node || (link_type(node->next))->next == node) {
            return;
        }

        iterator first = begin();
        ++first;
        while (first != end()) {
            iterator old = first;
            ++first;
            transfer(begin(), old, first);
        }
    }

    void sort()
    {
        // can usr size func, but low efficiency
        if (node->next == node || (link_type(node->next))->next == node) {
            return;
        }

         list<T, Alloc> carry;
         list<T, Alloc> counter[64];
         int fill = 0;
         while (!empty()) {
             carry.splice(carry.begin(), *this, begin());
             int i = 0;
             while (i < fill && ! counter[i].empty()) {
                counter[i].merge(carry);
                carry.swap(counter[i++]);
             }
         }
    }

    void swap(list<T, Alloc>& x)
    {

    }

    void merge(list<T, Alloc>& x)
    {
        iterator first1 = begin();
        iterator end1 = end();
        iterator first2 = x.begin();
        iterator end2 = x.end();

        while (first1 != end1 && first2 != end2) {
            if (*first2 < *first1) {
                iterator next = first2;
                transfer(first1, first2, ++next);
                first2 = next;
            }
            else {
                ++first1;
            }

            if (first2 != end2) {
                transfer(end1, first2, end2);
            }
        }
    }

private:
    link_type node;

    link_type get_node()
    {
        return list_allocator::allocate();
    }

    void put_node(link_type p)
    {
        list_allocator::deallocate(p);
    }

    link_type create_node(const T& x)
    {
        link_type p = this->get_node();
        construct(&p->data, x);
        return p;
    }

    void destroy_node(link_type p)
    {
        destroy(&p->data);
        this->put_node(p);
    }

    void empty_initialize()
    {
        node = get_node();
        node->next = node;
        node->prev = node;
    }

    void transfer(iterator pos, iterator first, iterator last)
    {
        if (pos != last) {
            (*(link_type((*last.node).prev))).next = pos.node;
            (*(link_type((*first.node).prev))).next = last.node;
            (*(link_type((*pos.node).prev))).next = first.node;
            link_type tmp = link_type((*pos.node).prev);
            (*pos.node).prev = (*last.node).prev;
            (*last.node).prev = (*first.node).prev;
            (*first.node).prev = tmp;
        }
    }
};
}
#endif // LIST

