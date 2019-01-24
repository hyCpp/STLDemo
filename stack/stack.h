#ifndef STACK
#define STACK

#include <deque/deque.h>

namespace STL {


template<typename T, typename Sequence = deque<T> >
class stack {
public:
    typedef typename Sequence::value_type value_type;
    typedef typename Sequence::size_type size_type;
    typedef typename Sequence::reference reference;

protected:
    Sequence c;
public:
    bool empty() const {
        return c.empty();
    }

    size_type size() const {
        return c.size();
    }

    reference top() {
        return c.back();
    }

    void push(const value_type& val) {
        c.push_back(val);
    }

    void pop() {
        c.pop_back();
    }

};

}

#endif // STACK

