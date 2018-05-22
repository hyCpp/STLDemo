#ifndef PAIR
#define PAIR

namespace STL {

template<typename T1, typename T2>
struct pair {
    typedef T1 first_type;
    typedef T2 second_type;

    T1 first;
    T2 second;

    pair()
        : first(T1())
        , second(T2())
    {
    }

    pair(const T1& a, const T2& b)
        : first(a)
        , second(b)
    {
    }

    pair& operator=(const pair& p)
    {
        first = p.first;
        second = p.second;
        return *this;
    }


    template<class _U1, class _U2>
    pair& operator=(const pair<_U1, _U2>& p)
    {
        first = p.first;
        second = p.second;
        return *this;
    }

};

}
#endif // PAIR

