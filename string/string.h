#ifndef STRING
#define STRING
#include <iostream>

namespace STL
{

class string
{
public:
    // construct
    string(const char* str = nullptr)
    {

    }

    string(const string& str)
    {

    }

    string(int n, char c)
    {

    }

    ~string()
    {

    }

    // Character operation

    const char &operator [] (int n) const
    {

    }

    const char &at(int n) const
    {

    }

    char &operator [] (int n)
    {

    }

    const char *c_str() const
    {

    }

    // Character description

    int size() const
    {

    }

    int length() const
    {

    }

    bool empty() const
    {

    }

    // Character assignment
    string &operator = (const string &s)
    {

    }

    string &assign(const char *s)
    {

    }

    string &assign(const string& str)
    {

    }

    // Character connection
    string &operator += (const string &str)
    {

    }

    string &append(const char *s)
    {

    }

    string &append(const string &str)
    {

    }


    // character compare

//    bool operator == (const string &s1, const string &s2)
//    {
//        return false;
//    }

    int compare(const string &str)
    {

    }

    int compare(int pos, int n, const string &str)
    {

    }

    int find(char* str, int pos = 0)
    {

    }

    int find(const string& str, int pos = 0)
    {

    }

    friend std::istream& operator >> (std::istream &is, string &str)
    {

    }

    friend std::ostream& operator << (std::ostream &os, string &str)
    {

    }

private:
    char* m_data;
    size_t m_length;
};

}
#endif // STRING

