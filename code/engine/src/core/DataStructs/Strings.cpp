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
        sz = strlen(c);
        internal_string = new char[sz];
        strncpy(internal_string, c, sz);
    }

    String::String(const String &s)
    {
        sz = s->length();
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

    std::ostream& operator<<(std::ostream& os, const String& s)
    {

    }

    String& operator= (const String& s)
    {

    }

    b8 String::is_equal(String s1, String s2)
    {

    }

    b8 String::is_equal_i(String s1, String s2)
    {

    }

    u32 String::length(void) const
    {

    }

}