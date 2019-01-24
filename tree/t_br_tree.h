#ifndef T_BR_TREE
#define T_BR_TREE

#include "t_iterator.h"
#include "t_allocator.h"
#include "t_construct.h"
#include "pair/pair.h"
#include "t_stl_func.h"
#include <memory>

#include <string>
using namespace STL;
typedef bool __rb_tree_color_type;
const __rb_tree_color_type __rb_tree_red = false;
const __rb_tree_color_type __rb_tree_black = true;

struct __rb_tree_node_base
{
    typedef __rb_tree_color_type    color_type;
    typedef __rb_tree_node_base*    base_ptr;

    color_type  color;
    base_ptr    parent;
    base_ptr    left;
    base_ptr    right;

    static base_ptr minimum(base_ptr ptr)
    {
        while (ptr->left != 0) {
            ptr = ptr->left;
        }
        return ptr;
    }

    static base_ptr maximum(base_ptr ptr)
    {
        while (ptr->right != 0) {
            ptr = ptr->right;
        }
        return ptr;
    }
};

template<typename T>
struct __rb_tree_node : public __rb_tree_node_base
{
    typedef __rb_tree_node<T>* link_type;
    T value_field;
};

struct __rb_tree_base_iterator
{
    typedef __rb_tree_node_base::base_ptr    base_ptr;
    typedef bidirectional_iterator_tag       iterator_category;
    typedef std::ptrdiff_t                   difference_type;

    base_ptr node;

    /**
        only use by operator++
        Find thie smallest node larger than ther current node
    */
    void increment()
    {
        if (node->right != 0) {
            node = node->right;
            while (node->left != 0) {
                node = node->left;
            }
        }
        else {
            base_ptr tmp = node->parent;
            while (node == tmp->right) {
                node = tmp;
                tmp = tmp->parent;
            }

            // The current node is the root node, and the root node has no right subnode
            if (node->right != tmp) {
                node = tmp;
            }
        }
    }

    /**
        only use by operator--
        Find thie largest node smaller than ther current node
    */
    void decrement()
    {
        // The parent node of the parent of the root node points to the root node
        // The current ierator points to the end;
        if (node->color == __rb_tree_red && node->parent->parent == node) {
            node = node->right;
        }
        else if (node->left != 0){
            base_ptr tmp = node->left;
            while (tmp->right != 0) {
                tmp = tmp->right;
            }
            node = tmp;
        }
        else {
            base_ptr tmp = node->parent;
            while (tmp->left == node) {
                node = tmp;
                tmp = tmp->parent;
            }
            node = tmp;
        }
    }
};

template<typename T, typename Ref, typename Ptr>
struct __rb_tree_iterator : public __rb_tree_base_iterator
{
    typedef T                                           value_type;
    typedef Ref                                         reference;
    typedef Ptr                                         pointer;
    typedef __rb_tree_iterator<T, T&, T*>               iterator;
    typedef __rb_tree_iterator<T, const T&, const T*>   const_iterator;
    typedef __rb_tree_iterator<T, Ref, Ptr>             self;
    typedef __rb_tree_node<T>*                          link_type;
    typedef bidirectional_iterator_tag                  iterator_category;
    typedef std::ptrdiff_t                              difference_type;

    __rb_tree_iterator() {}
    __rb_tree_iterator(link_type t)
    {
        node = t;
    }
    __rb_tree_iterator(const iterator& it)
    {
        node = it.node;
    }

//    __rb_tree_iterator& operator = (const iterator& it)
//    {
//        node = it.node;
//    }

    reference operator*() const
    {
        return static_cast<link_type>(node)->value_field;
    }

    pointer operator->() const
    {
        return &(operator * ());
    }

    self& operator++()
    {
        increment();
        return *this;
    }

    self operator++(int)
    {
        self tmp = *this;
        increment();
        return tmp;
    }

    self& operator--()
    {
        decrement();
        return *this;
    }

    self operator--(int)
    {
        self tmp = *this;
        decrement();
        return tmp;
    }

    bool operator==(const self& x) const
    {
        return x.node == node;
    }

    bool operator != (const self& x) const
    {
        return x.node != node;
    }
};





template<typename Key, typename Val, typename KeyOfVal, typename Compare, typename Alloc = default_alloc>
class rb_tree
{
private:
    typedef void*                                   void_pointer;
    typedef __rb_tree_node_base*                    base_ptr;
    typedef __rb_tree_node<Val>                     rb_tree_node;
    typedef simple_alloc<rb_tree_node, Alloc>       rb_tree_node_allocator;
    typedef __rb_tree_color_type                    color_type;

public:
    typedef Key                 key_value;
    typedef Val                 value_type;
    typedef value_type*         pointer;
    typedef const value_type*   const_pointer;
    typedef value_type&         reference;
    typedef const value_type&   const_reference;
    typedef rb_tree_node*       link_type;
    typedef size_t              size_type;
    typedef std::ptrdiff_t      difference_type;

private:
    link_type get_node()
    {
        return rb_tree_node_allocator::allocate();
    }

    void put_node(link_type pNode)
    {
       return rb_tree_node_allocator::deallocate(pNode);
    }

    link_type create_node(const value_type& type)
    {
        link_type tmp = get_node();
        try {
            construct(&tmp->value_field, type);
        }
        catch (...) {
            put_node(tmp);
            throw;
        }

        return tmp;
    }

    void destory_node(link_type pNode)
    {
        destory_node(&pNode->value_field);
        put_node(pNode);
    }

    link_type clone_node(link_type pNode)
    {
        link_type tmp = create_node(pNode->value_field);
        tmp->color = pNode->color;
        tmp->left = 0;
        tmp->right = 0;
        return tmp;
    }

protected:
    size_type node_count;
    link_type header;
    Compare   key_compare;

    link_type& root() const
    {
        return (link_type&)header->parent;
    }

    link_type& leftmost() const
    {
        return (link_type&)header->left;
    }

    link_type& rightmost() const
    {
        return (link_type&)header->right;
    }

    static link_type& left(link_type pNode)
    {
        return (link_type&)pNode->left;
    }
    static link_type& right(link_type pNode)
    {
        return (link_type&)pNode->right;
    }
    static link_type& parent(link_type pNode)
    {
        return (link_type&)pNode->parent;
    }
    static reference& value(link_type pNode)
    {
        return pNode->value_field;
    }
    static const Key& key(link_type pNode)
    {
        return KeyOfVal()(value(pNode));
    }
    static color_type& color(link_type pNode)
    {
        return static_cast<color_type&>(pNode->color);
    }


    static link_type& left(base_ptr pNode)
    {
        return (link_type&)pNode->left;
    }
    static link_type& right(base_ptr pNode)
    {
        return (link_type&)pNode->right;
    }
    static link_type& parent(base_ptr pNode)
    {
        return (link_type&)pNode->parent;
    }
    static reference& value(base_ptr pNode)
    {
        return static_cast<link_type>(pNode)->value_field;
    }
    static const Key& key(base_ptr pNode)
    {
        return KeyOfVal()(value((link_type)pNode));
    }
    static color_type& color(base_ptr pNode)
    {
        return static_cast<color_type&>(static_cast<link_type>(pNode)->color);
    }

    static link_type maximum(link_type pNode)
    {
        return static_cast<link_type>(__rb_tree_node_base::maximum(pNode));
    }
    static link_type minimum(link_type pNode)
    {
        return static_cast<link_type>(__rb_tree_node_base::minimum(pNode));
    }

public:
    typedef __rb_tree_iterator<value_type, reference, pointer> iterator;
    typedef __rb_tree_iterator<value_type, const_reference, const_pointer> const_iterator;

private:
    void init()
    {
        header = get_node();
        color(header) = __rb_tree_red;
        root() = 0;
        leftmost() = header;
        rightmost() = header;
    }

    iterator __insert(base_ptr _x, base_ptr _y, const value_type& value) // insert one element
    {
        link_type x = static_cast<link_type>(_x);
        link_type y = static_cast<link_type>(_y);
        link_type z;

        if (y == header || x != 0 || key_compare(KeyOfVal()(value), key(y))) {
            z = create_node(value);
            left(y) = z;
            if (y == header) {
                root() = z;
                rightmost() = z;
            }
            else if (y == leftmost()) {
                leftmost() = z;
            }
        }
        else {
            z = create_node(value);
            right(y) = z;
            if (y == rightmost()) {
                rightmost() = z;
            }
        }

        parent(z) = y;
        left(z) = 0;
        right(z) = 0;
        __rb_tree_rebalance(z, header->parent);
        ++node_count;
        return iterator(z);
    }

//    link_type __copy(link_type x, link_type p);

    void __erase_all(link_type x)
    {
        while (x != 0)
        {
            __erase_all(right(x));
            link_type y = left(x);
            destroy(x);
            x = y;
        }
    }

    void _erase_aux(const const_iterator pos)
    {
//        link_type y = static_cast<link_type>();
    }

    inline void __rb_tree_rotate_left(__rb_tree_node_base* x, __rb_tree_node_base*& root)
    {
        __rb_tree_node_base* y = x->right;

        x->right = y->left;
        if (y->left != 0) {
            y->left->parent = x;
        }
        y->parent = x->parent;

        // Replacing y node with x node(inheriting the relationship of x node)
        if (x == root) {
            root = y;
        }
        else if (x == x->parent->left) {
            x->parent->left = y;
        }
        else {
            x->parent->right = y;
        }

        y->left = x;
        x->parent = y;
    }


    inline void __rb_tree_rotate_right(__rb_tree_node_base* x, __rb_tree_node_base*& root)
    {
        __rb_tree_node_base* y = x->left;

        x->left = y->right;
        if (y->right != 0) {
            y->right->parent = x;
        }
        y->parent = x->parent;

        // Replacing y node with x node(inheriting the relationship of x node)
        if (x == root) {
            root = y;
        }
        else if (x == x->parent->right) {
            x->parent->right = y;
        }
        else {
            x->parent->left = y;
        }

        y->right = x;
        x->parent = y;
    }


    inline void __rb_tree_rebalance(__rb_tree_node_base* x, __rb_tree_node_base*& root)
    {
        x->color = __rb_tree_red; // new node is must red

        while (x != root && x->parent->color == __rb_tree_red) {
            if (x->parent == x->parent->parent->left) {
                __rb_tree_node_base* y = x->parent->parent->right;

                /**
                  case 3 4:
                    x->parent == x->parent->parent->left
                    x->parent->parent->right != null
                    x->parent->parent->right is red
                */
                if (y && y->color == __rb_tree_red) {
                    x->parent->color = __rb_tree_black;
                    y->color = __rb_tree_black;
                    x->parent->parent->color = __rb_tree_red;
                    x = x->parent->parent; // loop; inter case 1 2 or case 3 4
                }
                /**
                  case 1 2:
                    x->parent == x->parent->parent->left
                    x->parent->parent->right == null
                    x->parent->parent->right is black
                */
                else {
                    if (x == x->parent->right) { // case 2 : new node is right node
                        x = x->parent;
                        __rb_tree_rotate_left(x, root);
                    }

                    // case 1 : new node is left node
                    x->parent->color = __rb_tree_black;
                    x->parent->parent->color = __rb_tree_red;
                    __rb_tree_rotate_right(x->parent->parent, root);
                }
            }
            else {
                // And the above symmetry
                __rb_tree_node_base* y = x->parent->parent->left;

                if (y && y->color == __rb_tree_red) {
                    x->parent->color = __rb_tree_black;
                    y->color = __rb_tree_black;
                    x->parent->parent->color = __rb_tree_red;
                    x = x->parent->parent;
                }
                else {
                    if (x == x->parent->left) {
                        x = x->parent;
                        __rb_tree_rotate_right(x, root);
                    }

                    x->parent->color = __rb_tree_black;
                    x->parent->parent->color = __rb_tree_red;
                    __rb_tree_rotate_left(x->parent->parent, root);
                }
            }
        }

        root->color = __rb_tree_black;
    }

public:
    rb_tree(const Compare& com = Compare())
        : node_count(0)
        , key_compare(com)
    {
        init();
    }

    rb_tree<Key, Val, KeyOfVal, Compare, Alloc>& operator = (const rb_tree<Key, Val, KeyOfVal, Compare, Alloc>&);

    ~rb_tree()
    {
        clear();
        put_node(header);
    }

    Compare key_comp() const
    {
        return key_compare;
    }

    void clear()
    {
        if (node_count != 0) {
            __erase_all(root());
            leftmost() = header;
            root() = 0;
            rightmost() = header;
            node_count = 0;
        }
    }

    iterator begin()
    {
        return leftmost();
    }

    const_iterator begin() const
    {
        return leftmost();
    }

    iterator end()
    {
        return header;
    }

    const_iterator end() const
    {
        return header;
    }

    bool empty() const
    {
        return node_count == 0;
    }

    size_type size() const
    {
        return node_count;
    }

    size_type count(const value_type& v)
    {
        pair<iterator, iterator> __p = equal_range(v);
        size_type __n = distance(__p.first, __p.second);
        return __n;
    }

    void erase(const key_value* first, const key_value* last)
    {
        while (first != last) {
            erase(*first++);
        }
    }

    size_type erase(const key_value& x)
    {
        pair<iterator, iterator> p = equal_range(x);
        const size_type old_size = size();
        erase(p.first, p.second);
        return old_size - size();
    }

    iterator erase(iterator __position)
    {
        iterator __result = __position;
        ++__result;
        _erase_aux(__position);
        return __result;
    }

    size_type max_size() const
    {
        return size_type(-1);
    }

    void swap(rb_tree& t)
    {
        if (root() == 0) {
            if (t.root() != 0) {
                root() = t.root();
                rightmost() = t.rightmost();
                leftmost() = t.leftmost();
                root()->parent = end();

                t.root() = 0;
                t.leftmost() = t.end();
                t.rightmost() = t.end();
            }
        }
        else if (t.root() == 0) {
            t.root() = root();
            t.leftmost() = leftmost();
            t.rightmost() = rightmost();
            t.root()->parent = t.end();

            root() = 0;
            leftmost() = end();
            rightmost() = end();
        }
        else {
            std::swap(root(), t.root());
            std::swap(leftmost(), t.leftmost());
            std::swap(rightmost(), t.rightmost());

            root()->parent = end();
            t.root()->parent = t.end();
        }

        std::swap(this->node_count, t.node_count);
        std::swap(this->key_compare, t.key_compare);
    }

    iterator lower_bound(link_type __x, link_type __y, const value_type& __k)
    {
        while (__x != 0) {
            if (!key_compare(key(__x), __k))
                __y = __x, __x = left(__x);
            else
                __x = right(__x);
        }
        return iterator(__y);
    }

    iterator upper_bound(link_type __x, link_type __y, const value_type& __k)
    {
        while (__x != 0) {
            if (key_compare(__k, key(__x)))
                __y = __x, __x = left(__x);
            else
                __x = right(__x);
        }
        return iterator(__y);
    }


    iterator lower_bound(const value_type& __k)
    { return lower_bound(begin(), end(), __k); }

    iterator upper_bound(const value_type& __k)
    { return upper_bound(begin(), end(), __k); }

    pair<iterator, iterator> equal_range(const key_value& value)
    {
        link_type __x = leftmost();
        link_type __y = header;
        while (__x != 0)
        {
            if (key_compare(key(__x), value))
                __x = right(__x);
            else if (key_compare(value, key(__x)))
                __y = __x, __x = left(__x);
            else
            {
                link_type __xu(__x), __yu(__y);
                __y = __x, __x = left(__x);
                __xu = right(__xu);
                return pair<iterator, iterator>(lower_bound(__x, __y, value), upper_bound(__xu, __yu, value));
            }
        }
        return pair<iterator, iterator>(iterator(__y), iterator(__y));
    }

    pair<iterator, bool> insert_unique(const value_type& value) // key unique, confirm insert position
    {
        link_type y = header;
        link_type x = root();
        bool comp = true;

        while (x != 0) {
            y = x;
            comp = key_compare(KeyOfVal()(value), key(x));
            x = comp ? left(x) : right(x);
        }

        iterator j = iterator(y);
        if (comp) {  //left
            if (j == begin()) { //  parent node of x is most left node
                return pair<iterator, bool>(__insert(x, y, value), true);
            }
            else {
                --j;
            }
        }

        if (key_compare(key(j.node), KeyOfVal()(value))) { // Greater than left node
            return pair<iterator, bool>(__insert(x, y, value), true);
        }

        return pair<iterator, bool>(j, false); // key repeats, and do not insert new nodes
    }

    iterator insert_unique(const_iterator pos, const value_type& value)
    {
        if (pos.node == header) {
            if (size() > 0 && key_compare(key(rightmost()), KeyOfVal()(value))) {
                return __insert(0, rightmost(), value);
            }
            else {
                return insert_unique(value).first;
            }
        }
        else if (key_compare(KeyOfVal()(value), key(pos.node))) {
            const_iterator before = pos;

            if (pos.node == leftmost()) {
                return __insert(leftmost(), leftmost(),value);
            }
            else if (key_compare(key((--before).node), KeyOfVal()(value))) {
                if (right(before.node) == 0) {
                    return __insert(0, before.node, value);
                }
                else {
                    return __insert(pos.node, pos.node, value);
                }
            }
        }
        else if (key_compare(key(pos.node), KeyOfVal()(value))) {
            return insert_unique(value).first;
        }
    }

    template<typename InputIterator>
    iterator insert_unique(InputIterator first, InputIterator last)
    {
        for (; first != last; ++first) {
            insert_unique(end(), *first);
        }
    }

    iterator insert_equal(const value_type& value) // key can be equal
    {
        link_type y = header;
        link_type x = root();
        while (x != 0) {
            y = x;
            x = key_compare(KeyOfVal()(value), key(x)) ? left(x) : right(x); // key_compare is func object, compare node value
        }

        return __insert(x, y, value);
    }

    iterator find(const Key& k)
    {
        link_type y = header;
        link_type x = root();

        while (x != 0) {
            if (!key_compare(key(x), k)) {
                y = x;
                x = left(x);
            }
            else {
                x =right(x);
            }
        }

        iterator iter = iterator(y);
        return (iter == end() || key_compare(k, key(iter.node))) ? end() : iter;
    }
};

#endif // T_BR_TREE

