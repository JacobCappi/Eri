#include <cstring>
#include <ostream>

#include "defines.h"

namespace ERI
{
    class String
    {
    private:
    // TODO: migrate to a smart pointer? shared pointer seems perfect
    // I think I can only support deep copy until unique pointers 
        char *internal_string;
        u32 sz;

    public:
        String();
        String(char c);
        String(const char *c);

        String(const String &s); ///< Copy Constructor (Deep)
        ~String();

        char String::operator[] (u32 i) const;
        char& String::operator[] (u32 i);

        friend std::ostream& operator<<(std::ostream& os, const String& s);
        String& operator= (const String& s);

        b8 is_equal(String s1, String s2);
        b8 is_equal_i(String s1, String s2);
        u32 length(void) const;
    };
}