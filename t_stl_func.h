#ifndef T_STL_FUNC
#define T_STL_FUNC

namespace STL {

template<typename _Arg, typename _Result>
struct unary_function
{
    /// @c argument_type is the type of the argument
    typedef _Arg 	argument_type;

    /// @c result_type is the return type
    typedef _Result 	result_type;
};

template<typename _Arg1, typename _Arg2, typename _Result>
struct binary_function
{
    /// @c first_argument_type is the type of the first argument
    typedef _Arg1 	first_argument_type;

    /// @c second_argument_type is the type of the second argument
    typedef _Arg2 	second_argument_type;

    /// @c result_type is the return type
    typedef _Result 	result_type;
};

template<typename T>
struct identity : public unary_function<T, T>
{
    const T& operator() (const T& x) const
    {
        return x;
    }
};

template<typename _Tp>
struct less : public binary_function<_Tp, _Tp, bool>
{
    bool operator()(const _Tp& __x, const _Tp& __y) const
    {
        return __x < __y;
    }
};

template<typename _Tp>
struct greater : public binary_function<_Tp, _Tp, bool>
{
    bool operator()(const _Tp& __x, const _Tp& __y) const
    {
        return __x > __y;
    }
};

template<typename _Tp>
struct equal_to : public binary_function<_Tp, _Tp, bool>
{
    bool
    operator()(const _Tp& __x, const _Tp& __y) const
    { return __x == __y; }
};

template <typename T>
struct plus : public binary_function<T, T, T> {
    T operator () (const T& x, const T& y) const
    {
        return x + y;
    }
};

template <typename T>
struct minus : public binary_function<T, T, T> {
    T operator () (const T& x, const T& y) const
    {
        return x - y;
    }
};

template <typename T>
struct multiplies : public binary_function<T, T, T> {
    T operator () (const T& x, const T& y) const
    {
        return x * y;
    }
};

template <typename T>
struct divides : public binary_function<T, T, T> {
    T operator () (const T& x, const T& y) const
    {
        return x / y;
    }
};

template <typename T>
struct modulus : public binary_function<T, T, T> {
    T operator () (const T& x, const T& y) const
    {
        return x % y;
    }
};

template <typename Pair>
struct select1st : public unary_function<Pair, typename Pair::first_type>
{
    const typename Pair::first_type& operator () (const Pair& x) const {
        return x.first;
    }
};

template <typename Pair>
struct select2st : public unary_function<Pair, typename Pair::second_type>
{
    const typename Pair::second_type& operator () (const Pair& x) const {
        return x.second;
    }
};

template <typename Arg1, typename Arg2>
struct project1st : public binary_function<Arg1, Arg2, Arg1> {
    Arg1 operator () (const Arg1& x, const Arg2&) const
    {
        return x;
    }
};

template <typename Arg1, typename Arg2>
struct project2st : public binary_function<Arg1, Arg2, Arg2> {
    Arg2 operator () (const Arg1&, const Arg2& x) const
    {
        return x;
    }
};

}
#endif // T_STL_FUNC

