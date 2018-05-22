#ifndef SET
#define SET
#include "t_br_tree.h"

namespace STL {

template<typename Key, typename Compare = less<Key>, typename Alloc = default_alloc >
class set
{
public:
    typedef Key                 key_type;
    typedef Key                 value_type;
    typedef Compare             key_compare;
    typedef Compare             value_compare;

private:
    typedef rb_tree<key_type, value_type, identity<value_type>, key_compare, Alloc> rep_type;

    rep_type t;
public:
    typedef typename rep_type::const_pointer    const_pointer;
    typedef typename rep_type::const_pointer    pointer;
    typedef typename rep_type::const_reference  reference;
    typedef typename rep_type::const_reference  const_reference;
    typedef typename rep_type::const_iterator   iterator;
    typedef typename rep_type::const_iterator   const_iterator;
    typedef typename rep_type::size_type        size_type;
    typedef typename rep_type::difference_type  difference_type;

    set()
        : t(Compare())
    {
    }

    explicit set(const Compare& comp)
        : t(comp)
    {
    }

    template<typename InputIterator>
    set(InputIterator first, InputIterator last)
        : t(Compare())
    {
        t.insert_unique(first, last);
    }

    template<typename InputIterator>
    set(InputIterator first, InputIterator last, const Compare& comp)
        : t(comp)
    {
        t.insert_unique(first, last);
    }

    set(const set<Key, Compare, Alloc>& x)
        : t(x.t)
    {
    }

    set<Key, Compare, Alloc>& operator = (const set<Key, Compare, Alloc>& x)
    {
        t = x.t;
        return *this;
    }

    key_compare key_comp() const
    {
        return t.key_comp();
    }

    value_compare value_comp() const
    {
        return t.key_comp();
    }

    iterator begin() const
    {
        return t.begin();
    }

    iterator end() const
    {
        return t.end();
    }

    bool empty() const
    {
        return t.empty();
    }

    size_type size() const
    {
        return t.size();
    }

    size_type max_size() const
    {
        return t.max_size();
    }

    typedef pair<iterator, bool> pair_iterator_bool;
    pair<iterator, bool> insert(const value_type& x)
    {
        pair<typename rep_type::iterator, bool> p = t.insert_unique(x);
        return pair<iterator, bool>(p.first, p.second);
    }

    iterator insert(iterator pos, const value_type& x)
    {
        typedef typename rep_type::iterator rep_iterator;
        return t.insert_unique((rep_iterator&)pos, x);
    }

    template<typename InputIterator>
    void insert(InputIterator first, InputIterator last)
    {
        t.insert_unique(first, last);
    }

    void erase(iterator pos)
    {
        typedef typename rep_type::iterator rep_iterator;
        t.erase((rep_iterator&)pos);
    }

    void clear()
    {
        t.clear();
    }

    iterator find(const key_type& x)
    {
        return t.find(x);
    }

    size_type count(const key_type& x)
    {
        return t.count(x);
    }

    iterator lower_bound(const key_type& x)
    {
        return t.lower_bound(x);
    }

    iterator upper_bound(const key_type& x)
    {
        return t.upper_bound(x);
    }

    pair<iterator, iterator> equal_range(const key_type& x)
    {
        return t.equal_range(x);
    }
};

}

#endif // SET

