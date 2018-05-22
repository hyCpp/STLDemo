#ifndef T_STL_FUNC
#define T_STL_FUNC

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
struct equal_to : public binary_function<_Tp, _Tp, bool>
{
    bool
    operator()(const _Tp& __x, const _Tp& __y) const
    { return __x == __y; }
};


#endif // T_STL_FUNC

