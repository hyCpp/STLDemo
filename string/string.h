#ifndef STRING
#define STRING
#include <iostream>
#include <string.h>

// this is simple Realization, really is use OSS or COW

namespace STL
{

class string
{
public:
    // construct
    string()
        : m_data(nullptr)
        , m_length(0)
    {
    }

    string(const char* str)
    {
        if(nullptr == str) return;
        m_length = strlen(str);
        m_data = new char[m_length + 1];
        strcpy(m_data, str);
    }

    string(const string& str)
    {
        if (str.c_str() == nullptr) return;

        m_length = str.size();
        m_data = new char[m_length + 1];
        strcpy(m_data, str.c_str());
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
        return m_data;
    }

    // Character description

    int size() const
    {
        return m_length;
    }

    int length() const
    {
        return m_length;
    }

    bool empty() const
    {

    }

    // Character assignment
    string &operator = (const string &s)
    {
        if (this != &s) {
            if (s.size() > m_length) {
                delete[] m_data;
                m_data = new char[s.size() + 1];
            }
            m_length = s.size();
            strcpy(m_data, s.c_str());
        }
        return *this;
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

