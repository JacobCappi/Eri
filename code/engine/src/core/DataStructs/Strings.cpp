#include <cstring>

#include "Strings.h"

//TODO: integrate with memory layer
namespace ERI
{
    String::String()
    {
        internal_string = new char[0];
        sz = 0;
    }

    String::String(char c)
    {
        internal_string = new char(c);
        sz = 1;
    }

    String::String(const char *c)
    {
        if (!c)
        {
            sz = 0;
            return;
        }

        sz = strlen(c);
        internal_string = new char[sz];
        strncpy(internal_string, c, sz);
    }

    String::String(const String &s)
    {
        sz = s.length();
        internal_string = new char[sz];
        for (u32 i = 0; i < sz; ++i)
        {
            internal_string[i] = s[i];
        }
    }

    char String::operator[] (u32 i) const 
    {
        if (i >= sz)
        {
            return -1;
        }
        return internal_string[i];
    }

    char& String::operator[] (u32 i) 
    {
        if (i >= sz) throw -1;
        return internal_string[i];
    }

    String::~String()
    {
        delete[] internal_string;
    }

    std::ostream& operator<<(std::ostream& o, const String& s)
    {
        if (s.length() <= 0)
        {
            o << "";
            return o;
        }

        // TODO: check if this is effecient?
        for (u32 i=0, size = s.length(); i < size; ++i)
        {
            o << s[i];
        }
        return o;
    }

    b8 String::is_equal(String s)
    {
        if (s.length() != sz)
        {
            return FALSE;
        }

        for (u32 i = 0, size = s.length(); i < size; ++i)
        {
            if (internal_string[i] != s[i])
            {
                return FALSE;
            }
        }

        return TRUE;
    }

    b8 String::is_equal_i(String s)
    {
        if (s.length() != sz)
        {
            return FALSE;
        }

        for (u32 i = 0, size = s.length(); i < size; i++)
        {
            if (tolower(s[i]) != tolower(internal_string[i]))
            {
                return FALSE;
            }
        }

        return TRUE;
    }

    u32 String::length(void) const
    {
        return sz;
    }

    void String::copy(const String &s)
    {
        delete[] internal_string;
        sz = s.length();
        internal_string = new char[sz];
        for (u32 i = 0, size = s.length(); i < size; ++i)
        {
            internal_string[i] = s[i];
        }
        return;
    }

}