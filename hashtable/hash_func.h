#ifndef HASH_FUNC
#define HASH_FUNC

#include <bits/c++config.h>
#include <string.h>

using std::size_t;

template<typename Key>
struct hash {

};

inline size_t __stl_hash_string(const char* s)
{
    unsigned long h = 0;
    for (; *s; ++s) {
        h = 5 * h + *s;
    }
    return size_t(h);
}

template<>
struct hash<char*>
{
    size_t operator()(const char* __s) const
    {
        return __stl_hash_string(__s);
    }
};

template<>
struct hash<const char*>
{
    size_t operator()(const char* __s) const
    {
        return __stl_hash_string(__s);
    }
};

template<>
struct hash<char>
{
    size_t operator()(char __x) const
    {
        return __x;
    }
};

template<>
struct hash<unsigned char>
{
    size_t operator()(unsigned char __x) const
    {
        return __x;
    }
};

template<>
struct hash<signed char>
{
    size_t operator()(unsigned char __x) const
    {
        return __x;
    }
};

template<>
struct hash<short>
{
    size_t operator()(short __x) const
    {
        return __x;
    }
};

template<>
struct hash<unsigned short>
{
    size_t operator()(unsigned short __x) const
    {
        return __x;
    }
};

template<>
struct hash<int>
{
    size_t operator()(int __x) const
    {
        return __x;
    }
};

template<>
struct hash<unsigned int>
{
    size_t operator()(unsigned int __x) const
    {
        return __x;
    }
};

template<>
struct hash<long>
{
    size_t operator()(long __x) const
    {
        return __x;
    }
};

template<>
struct hash<unsigned long>
{
    size_t operator()(unsigned long __x) const
    {
        return __x;
    }
};

struct eqstr
{
    bool operator()(const char* x, const char* y) const
    {
        return strcmp(x, y) == 0;
    }
};


#endif // HASH_FUNC

