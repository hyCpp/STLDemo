#ifndef HASHTABLE
#define HASHTABLE

#include "t_iterator.h"
#include "t_allocator.h"
#include "vector/vector.h"
#include "pair/pair.h"
#include "hash_func.h"

template<typename Value>
struct __hashtable_node
{
    __hashtable_node* next;
    Value val;
};


template<typename Value, typename Key, typename HashFcn, typename ExtractKey, typename EqualKey, typename Alloc = default_alloc>
class hashtable;


template<typename Value, typename Key, typename HashFcn, typename ExtractKey, typename EqualKey, typename Alloc>
struct __hashtable_iterator;


template<typename Value, typename Key, typename HashFcn, typename ExtractKey, typename EqualKey, typename Alloc>
struct __hashtable_const_iterator;


template<typename Value, typename Key, typename HashFcn, typename ExtractKey, typename EqualKey, typename Alloc>
struct __hashtable_iterator
{
    typedef hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>                     _hashtable;
    typedef __hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>          iterator;
    typedef __hashtable_const_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>    const_iterator;
    typedef __hashtable_node<Value>                                                         node;

    typedef forward_iterator_tag           iterator_category;
    typedef Value                          value_type;
    typedef std::ptrdiff_t                 difference_type;
    typedef size_t                         size_type;
    typedef Value&                         reference;
    typedef Value*                         pointer;

    node* cur;
    _hashtable* ht;

    __hashtable_iterator(node* n, _hashtable* tab)
        : cur(n)
        , ht(tab) {}

    __hashtable_iterator() {}

    reference operator* () const
    {
        return cur->val;
    }

    pointer operator -> () const
    {
        return &(operator*());
    }

    iterator& operator ++()
    {
        const node* old = cur;
        cur = cur->next;
        if (!cur) {
            size_type bucket = ht->bkt_num(old->val);
            while (!cur && ++bucket < ht->buckets.size()) {
                cur = ht->buckets[bucket];
            }
        }
        return *this;
    }

    iterator operator ++(int)
    {
        iterator tmp = *this;
        ++*this;
        return tmp;
    }

    bool operator == (const iterator& it) const
    {
        return cur == it.cur;
    }
    bool operator != (const iterator& it) const
    {
        return cur != it.cur;
    }
};

template<typename Value, typename Key, typename HashFcn, typename ExtractKey, typename EqualKey, typename Alloc>
struct __hashtable_const_iterator
{
    typedef hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>                     _hashtable;
    typedef __hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>          iterator;
    typedef __hashtable_const_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>    const_iterator;
    typedef __hashtable_node<Value>                                                         node;

    typedef forward_iterator_tag           iterator_category;
    typedef Value                          value_type;
    typedef std::ptrdiff_t                 difference_type;
    typedef size_t                         size_type;
    typedef Value&                         reference;
    typedef Value*                         pointer;

    node* cur;
    _hashtable* ht;

    __hashtable_const_iterator(const node* n, const _hashtable* tab)
        : cur(n)
        , ht(tab) {}

    __hashtable_const_iterator() {}

    __hashtable_const_iterator(const iterator& __it)
    : cur(__it.cur), ht(__it.ht) { }

    reference operator* () const
    {
        return cur->val;
    }

    pointer operator -> () const
    {
        return &(operator*());
    }

    iterator& operator ++()
    {
        const node* __old = cur;
        cur = cur->next;
        if (!cur)
        {
            size_type bucket = ht->bkt_num(__old->val);
            while (!cur && ++bucket < ht->buckets.size())
                cur = ht->buckets[bucket];
        }
        return *this;
    }

    iterator operator ++(int)
    {
        iterator tmp = *this;
        ++*this;
        return tmp;
    }

    bool operator == (const iterator& it) const
    {
        return cur == it.cur;
    }
    bool operator != (const iterator& it) const
    {
        return cur != it.cur;
    }
};

static const int __stl_num_primes = 29;
static const unsigned long __stl_prime_list[__stl_num_primes] =
{
    5ul,          53ul,         97ul,         193ul,       389ul,
    769ul,        1543ul,       3079ul,       6151ul,      12289ul,
    24593ul,      49157ul,      98317ul,      196613ul,    393241ul,
    786433ul,     1572869ul,    3145739ul,    6291469ul,   12582917ul,
    25165843ul,   50331653ul,   100663319ul,  201326611ul, 402653189ul,
    805306457ul,  1610612741ul, 3221225473ul, 4294967291ul
};

inline unsigned long __stl_next_prime(unsigned long n)
{
    const unsigned long* first = __stl_prime_list;
    const unsigned long* last = __stl_prime_list + __stl_num_primes;
    const unsigned long* pos = std::lower_bound(first, last, n);
    return pos == last ? *(last - 1) : *pos;
}


template<typename Value, typename Key, typename HashFcn, typename ExtractKey, typename EqualKey, typename Alloc>
class hashtable
{
public:
    typedef HashFcn             hasher;         // hash func type(functor)
    typedef EqualKey            key_equal;      // value are equal(functor)
    typedef size_t              size_type;
    typedef std::ptrdiff_t      difference;
    typedef Value               value_type;
    typedef Key                 Key_type;
    typedef value_type*         pointer;
    typedef value_type&         reference;
    typedef const value_type*   const_pointer;
    typedef const value_type&   const_reference;

    typedef __hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc> iterator;
    typedef __hashtable_const_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc> const_iterator;

private:
    hasher  hash;
    key_equal equals;
    ExtractKey get_key;  // value from the node

    typedef __hashtable_node<Value> node;
    typedef simple_alloc<node, Alloc> node_allocator;

    STL::vector<node*, Alloc> buckets;
    size_type num_elements;

public:
  friend struct
  __hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>;

  friend struct
  __hashtable_const_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>;

public:
    hashtable(size_type n, const HashFcn& hf, const EqualKey& eql)
        : hash(hf)
        , equals(eql)
        , get_key(ExtractKey())
        , num_elements(0)
    {
        initialize_buckets(n);
    }

    hashtable(const hashtable& ht)
        : hash(ht.hash)
        , equals(ht.equals)
        , num_elements(0)
        , buckets(ht.buckets)
        , get_key(ht.get_key)
    {
        copy_from(ht);
    }

    hashtable& operator = (const hashtable& ht)
    {
        if (&ht != this)
          {
            clear();
            hash = ht.hash;
            equals = ht.equals;
            get_key = ht.get_key;
            copy_from(ht);
          }
        return *this;
    }

    ~hashtable()
    {
        clear();
    }

    size_type bucket_count() const
    {
        return buckets.size();
    }

    size_type max_buckets_count() const
    {
        return __stl_prime_list[__stl_num_primes - 1];
    }

    size_type elems_in_bucket(size_type __bucket)
    {
        size_type result = 0;
        for (node* n = buckets[__bucket]; n; n = n->next) {
            result += 1;
        }
        return result;
    }

    size_type size() const
    {
        return num_elements;
    }

    size_type max_size() const
    {
        return size_type(-1);
    }

    bool empty() const
    {
        return size() == 0;
    }

    void swap(hashtable& __ht)
    {
        std::swap(hash, __ht.hash);
        std::swap(equals, __ht.equals);
        std::swap(get_key, __ht.get_key);
        buckets.swap(__ht.buckets);
        std::swap(num_elements, __ht.num_elements);
    }

    void clear()
    {
        if (num_elements == 0) {
            return;
        }

        for (size_type __i = 0; __i < buckets.size(); ++__i)
        {
            node* __cur = buckets[__i];
            while (__cur != 0)
            {
                node* __next = __cur->next;
                delete_node(__cur);
                __cur = __next;
            }
            buckets[__i] = 0;
        }
        num_elements = 0;
    }

    iterator begin()
    {
        for (size_type i = 0; i < buckets.size(); ++i) {
            if (buckets[i]) {
                return iterator(buckets[i], this);
            }
        }

        return end();
    }

    iterator end()
    {
        return iterator(0, this);
    }

    const_iterator begin() const
    {
        for (size_type i = 0; i < buckets.size(); ++i) {
            if (buckets[i]) {
                return const_iterator(buckets[i], this);
            }
        }

        return end();
    }

    const_iterator end() const
    {
        return const_iterator(0, this);
    }

    iterator find(const Key_type& key)
    {
        size_type n = bkt_num_key(key);
        node* first;

        for (first = buckets[n]; first && !equals(get_key(first->val), key); first = first->next) {}

        return iterator(first, this);
    }

    STL::pair<iterator, bool> insert_unique(const value_type& obj)
    {
        resize(num_elements + 1);
        return insert_unique_noresize(obj);
    }

    iterator insert_equal(const value_type& obj)
    {
        resize(num_elements + 1);
        return insert_equal_noresize(obj);
    }

    // rebuild the table ???
    void resize(size_type num_elements_hints)
    {
        const size_type old = buckets.size();
        if (num_elements_hints > old) { //load factor > 1
            const size_type n  = next_size(num_elements_hints); // next Prime number
            if (n > old) {
                STL::vector<node*, Alloc> tmp(n, (node*)0); //create new vector, For vector data exchange, Finally released
                try {
                    for (size_type bucket = 0; bucket < old; ++bucket) {
                        node* first = buckets[bucket]; // create new bucket, For bucket data exchange, Finally released
                        while (first) {
                            size_type newBucket = bkt_num(first->val, n);
                            buckets[bucket] = first->next;
                            first->next = tmp[newBucket];
                            tmp[newBucket] = first;
                            first = buckets[bucket];
                        }
                    }
                    buckets.swap(tmp);
                }
                catch (...) {
                    for (size_type bucket = 0; bucket < tmp.size(); ++bucket) {
                        while (tmp[bucket]) {
                            node* next = tmp[bucket]->next;
                            delete_node(next);
                            tmp[bucket] = next;
                        }
                    }
                    throw;
                }
            }
        }
    }

    size_type bkt_num_key(const Key_type key) const
    {
        return bkt_num_key(key, buckets.size());
    }

    size_type bkt_num(const value_type& value) const
    {
        return bkt_num_key(get_key(value));
    }

    size_type bkt_num_key(const Key_type& key, size_t n) const
    {
        return hash(key) % n;
    }

    size_type bkt_num(const value_type& value, size_t n) const
    {
        return bkt_num_key(get_key(value), n);
    }

private:

    size_type next_size(size_type n)
    {
        return __stl_next_prime(n); // near n and greater than n
    }

    void initialize_buckets(size_type n)
    {
        const size_type n_buckets = next_size(n);
        buckets.reserve(n);
        buckets.insert(buckets.end(), n_buckets, (node*)0); // insert n_buckets element,and value is 0
        num_elements = 0;
    }

    node* new_node(const value_type& obj)
    {
        node* n = node_allocator::allocate();
        n->next = 0;
        try {
            construct(&n->val, obj);
            return n;
        } catch(...) {
            delete_node(n);
            throw;
        }
    }

    void* delete_node(node* n)
    {
        destroy(&n->val);
        node_allocator::deallocate(n);
    }

    void copy_from(const hashtable& ht)
    {
        buckets.clear();

        buckets.reserve(ht.buckets.size());
        buckets.insert(buckets.end(), ht.buckets.size(), (node*)0);

        try {
            for (size_type i = 0; i< ht.buckets.size(); ++i) {
                if (const node* cur = ht.buckets[i]) {
                    node* copy = new_node(cur->val);
                    buckets[i] = copy;

                    for (node* next = cur->next; next; next = cur->next) {
                        copy->next = new_node(next->val);
                        copy = copy->next;
                    }
                }
            }
            num_elements = ht.num_elements;
        } catch (...) {
            clear();
            throw;
        }
    }

    STL::pair<iterator, bool> insert_unique_noresize(const value_type& obj)
    {
        const size_type n = bkt_num(obj); // The position of the element(obj) in bucket
        node* first = buckets[n];

        for (node* cur = first; cur; cur = cur->next) {
            if (equals(get_key(cur->val), get_key(obj))) {
                return STL::pair<iterator, bool>(iterator(cur, this), false);
            }
        }
        node* tmp = new_node(obj);
        tmp->next = first;
        buckets[n] = tmp;
        ++num_elements;
        return STL::pair<iterator, bool>(iterator(tmp, this), true);
    }

    iterator insert_equal_noresize(const value_type& obj)
    {
        const size_type n = bkt_num(obj);
        node* first = buckets[n];

        for (node* cur = first; cur; cur = cur->next) {
            if (equals(get_key(cur->val), get_key(obj))) {
                node* tmp = new_node(obj);
                tmp->next = cur->next;
                cur->next = tmp;
                ++num_elements;
                return iterator(tmp, this);
            }
        }

        node* tmp = new_node(obj);
        tmp->next = first;
        buckets[n] = tmp;
        ++num_elements;
        return iterator(tmp, this);
    }
};

#endif // HASHTABLE

