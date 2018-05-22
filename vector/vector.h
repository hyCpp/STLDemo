#ifndef VECTOR
#define VECTOR
#include "t_allocator.h"
#include "t_construct.h"
#include "t_uninitialized.h"

namespace STL {
template<typename T, class Alloc = default_alloc >
class vector
{
public:
    typedef T                       value_type;
    typedef size_t                  size_type;
    typedef std::ptrdiff_t          difference_type;
    typedef value_type&             reference;
    typedef value_type*             iterator;
    typedef value_type*             pointer;
    typedef const value_type*       const_pointer;
    typedef const value_type&       const_reference;
    typedef const value_type*       const_iterator;

private:
    typedef simple_alloc<value_type, Alloc> data_allocator;
    iterator                                start;
    iterator                                finish;
    iterator                                end_of_storage;

    void deallocate()
    {
        if (start) {
            data_allocator::deallocate(start , end_of_storage - start);
        }
    }

    iterator allocate_and_fill(size_type n, const T& x)
    {
        iterator result = data_allocator::allocate(n);
        uninitialized_fill_n(result, n, x);
        return result;
    }

    void fill_initialize(size_type n, const T& value)
    {
        start = allocate_and_fill(n, value);
        finish = start + n;
        end_of_storage = finish;
    }

    void insert_aux(iterator pos, const T& value)
    {
        if (finish != end_of_storage) {
            construct(finish, *(finish - 1));
            ++finish;
            T copy = value;
            std::copy_backward(pos, finish - 2, finish - 1);
            *pos = copy;
        }
        else {
            const size_type old = size();
            const size_type len = old != 0 ? 2 * old : 1;

            iterator newStart = data_allocator::allocate(len);
            iterator newFinish = newStart;
            try {
                newFinish = uninitialized_copy(start, pos, newStart);
                construct(newFinish, value);
                ++newFinish;
                newFinish = uninitialized_copy(pos, finish, newFinish);
            }
            catch (...) {
                destroy(newStart, newFinish);
                data_allocator::deallocate(newStart, len);
                throw;
            }

            destroy(begin(), end());
            deallocate();
            start = newStart;
            finish = newFinish;
            end_of_storage = newStart + len;
        }
    }

public:
    vector()
        : start(0),
          finish(0),
          end_of_storage(0)
    {
    }

    vector(size_type n, const T& value)
    {
        fill_initialize(n, value);
    }

    vector(int n, const T& value)
    {
        fill_initialize(n, value);
    }

    vector(long n, const T& value)
    {
        fill_initialize(n, value);
    }

    vector(const vector &vec)
    {
        start = 0;
        finish = 0;
        end_of_storage = 0;

        for (;vec.begin() != vec.end(); ++vec.begin()) {
            push_back(*(vec.begin()));
        }
    }

    template<typename _InputIterator>
    vector(_InputIterator begin, _InputIterator end)
    {
        start = 0;
        finish = 0;
        end_of_storage = 0;

        for (; begin != end; ++begin) {
            push_back(*begin);
        }
    }

    explicit vector(size_type n)
    {
        fill_initialize(n, T());
    }

    ~vector()
    {
        destroy(start, finish);
        deallocate();
    }

    iterator begin()
    {
        return start;
    }


    iterator end()
    {
        return finish;
    }

    const_iterator begin() const
    {
        return start;
    }

    const_iterator end() const
    {
        return finish;
    }

    size_type size() const
    {
        return size_type(end() - begin());
    }

    size_type capacity()
    {
        return size_type(end_of_storage - begin());
    }

    bool empty() const
    {
        return begin() == end();
    }

    reference operator[] (size_type n)
    {
        return *(begin() + n);
    }

    reference front()
    {
        return *begin();
    }

    reference back()
    {
        return *(end() - 1);
    }

    reference at(size_type n)
    {
        return start[n];
    }

    void push_back(const T& value)
    {
        if (finish != end_of_storage) {
            construct(finish, value);
            ++finish;
        }
        else {
            insert_aux(end(), value);
        }
    }

    void pop_back()
    {
        --finish;
        destroy(finish);
    }

    iterator erase(iterator pos)
    {
        if (pos + 1 != end()) {
            std::copy(pos + 1, finish, pos);
        }
        -- finish;
        destroy(finish);
        return pos;
    }

    iterator erase(iterator first, iterator last)
    {
        iterator i = std::copy(last, finish, first);
        destroy(i, finish);
        finish = finish - (last - first);
        return first;
    }

    void resize(size_type new_size)
    {
        resize(new_size, T());
    }

    void resize(size_type new_size, const T& x)
    {
        if (new_size < size()) {
            erase(begin() + new_size, end());
        }
        else {
            insert(end(), new_size - size(), x);
        }
    }

    void clear()
    {
        erase(begin(), end());
    }

    void insert(iterator pos, size_type n, const T& value)
    {
        if (n != 0) {
            if (size_type(end_of_storage - finish) >= n) {
                T copy = value;
                const size_type elems_after = finish - pos;
                iterator old = finish;

                if (elems_after > n) {
                    uninitialized_copy(finish - n, finish, finish);
                    finish += n;
                    std::copy_backward(pos, old - n, old);
                    std::fill(pos, pos + n, copy);
                }
                else {
                    uninitialized_fill_n(finish, n - elems_after, copy);
                    finish += n - elems_after;
                    uninitialized_copy(pos, old, finish);
                    finish += elems_after;
                    std::fill(pos, old, copy);
                }
            }
            else {
                const size_type old_size = size();
                const size_type len = old_size + std::max(old_size, n);
                iterator newStart = data_allocator::allocate(len);
                std::cout <<*newStart<<std::endl;
                iterator newFinish = newStart;

                try {
                    newFinish = uninitialized_copy(start, pos, newStart);
                    newFinish = uninitialized_fill_n(newFinish, n, value);
                    newFinish = uninitialized_copy(pos, finish, newFinish);
                }
                catch (...) {
                    destroy(newStart, newFinish);
                    data_allocator::deallocate(newStart, len);
                    throw;
                }

                destroy(start, finish);
                deallocate();
                start = newStart;
                finish = newFinish;
                end_of_storage = newStart + len;
            }
        }
    }

    void swap(vector& v)
    {
        std::swap(start, v.start);
        std::swap(finish, v.finish);
        std::swap(finish, v.end_of_storage);
    }

    void reserve(size_type n)
    {
        if (n > max_size()) {
            std::cout<<"error : vector::reserve"<<std::endl;
            return;
        }

        if (n > this->capacity()) {
            const size_type old_size = size();
            pointer ptr = data_allocator::allocate(n);

            if (start != 0) {
                destroy(start, finish);
                data_allocator::deallocate(start, end_of_storage - start);
            }
            end_of_storage = ptr + n;
            finish = ptr + old_size;
            start = ptr;
        }
    }

    size_type max_size() const
    {
        return size_type(-1);
    }
};
}
#endif // VECTOR

