#ifndef MAP
#define MAP
#include "t_br_tree.h"
#include "pair/pair.h"

namespace STL {

template<typename Key, typename Value, typename Compare = less<Key>, typename Alloc = default_alloc >
class map
{
public:
    typedef Key                     key_type;
    typedef Value                   data_type;
    typedef Value                   mapped_type;
    typedef pair<const Key, Value>  value_type;
    typedef Compare                 key_compare;

    // define functor, For comparison of elements
    class value_compare : public binary_function<value_type, value_type, bool>
    {
        friend class map<Key, Value, Compare, Alloc>;
    protected:
        Compare comp;
        value_compare(Compare c)
            : comp(c)
        {
        }

    public:
        bool operator() (const value_type& v, const value_type& y) const
        {
            return comp(v.first, y.first);
        }
    };

private:
    typedef rb_tree<key_type, value_type, select1st<value_type>, key_compare, Alloc> rep_type;
    rep_type    t;

public:
    typedef typename rep_type::pointer              pointer;
    typedef typename rep_type::const_pointer        const_pointer;
    typedef typename rep_type::reference            reference;
    typedef typename rep_type::const_reference      const_reference;
    typedef typename rep_type::iterator             iterator;
    typedef typename rep_type::const_iterator       const_iterator;
    typedef typename rep_type::size_type            size_type;
    typedef typename rep_type::difference_type      difference_type;

    map()
        : t(Compare()) {}

    explicit map(const Compare& comp)
        : t(comp) {}

    template<typename InputIterator>
    map(InputIterator first, InputIterator last)
        : t(Compare())
    {
        t.insert_unique(first, last);
    }

    map(const map<Key, Value, Compare, Alloc>& x)
        : t(x.t) {}

    map<Key, Value, Compare, Alloc>& operator = (const map<Key, Value, Compare, Alloc>& x)
    {
        t = x.t;
        return *this;
    }

    Value& operator [] (const key_type& k)
    {
        return (*((insert(value_type(k, Value()))).first)).second;
    }

    key_compare key_comp() const
    {
        return t.key_comp();
    }

    value_compare value_comp() const
    {
        return value_compare(t.key_comp());
    }

    iterator begin()
    {
        return t.begin();
    }

    iterator end()
    {
        return t.end();
    }

    const_iterator begin() const
    {
        return t.begin();
    }

    const_iterator end() const
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

    void swap(map<Key, Value, Compare, Alloc>& x)
    {
        t.swap(x.t);
    }

    pair<iterator, bool> insert(const value_type& x)
    {
        return t.insert_unique(x);
    }

    iterator insert(iterator pos, const value_type& x)
    {
        return t.insert_unique(pos, x);
    }

    template<typename InputIterator>
    void insert(InputIterator first, InputIterator last)
    {
        t.insert_unique(first, last);
    }

    void erase(iterator pos)
    {
        t.erase(pos);
    }

    void clear()
    {
        t.clear();
    }

    iterator find(const key_type& x)
    {
        return t.find(x);
    }

    const_iterator find(const key_type &x) const
    {
        return t.find(x);
    }
};

}
#endif // MAP

