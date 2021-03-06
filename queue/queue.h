#ifndef QUEUE
#define QUEUE
#include "deque.h"
namespace STL {

// queue就是将deque包装一层
// queue没有迭代器
template <typename T, typename Sequence = deque<T> >
class queue
{
public:
    typedef typename Sequence::value_type value_type;
    typedef typename Sequence::size_type size_type;
    typedef typename Sequence::reference reference;

protected:
    Sequence c;

public:
    bool empty() const
    {
        return c.empty();
    }

    size_type size() const
    {
        return c.size();
    }

    reference front()
    {
        return c.front();
    }

    reference back()
    {
        return c.back();
    }

    void push(const value_type& x)
    {
        c.push_back(x);
    }

    void pop()
    {
        c.pop_front();
    }
};

}

#endif // QUEUE

