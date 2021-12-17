#pragma once
// NOTE: name is asserts*.h, as assert.h is a std lib header

#include "defines.h"

#define ERI_ASSERTIONS_ENABLED

#ifdef  ERI_ASSERTIONS_ENABLED
// Also from KOHI, addressed bug with different naming schemes
#if _MSC_VER
    #include <intrin.h>
    #define debugBreak() __debugbreak()
#else
    #define debugBreak() __builtin_trap()
#endif

ERI_API void report_assertion_failure(const char* bool_expression, const char* message, const char* origin_file, i32 line_number);

#define ERI_ASSERT(expression)                                              \
    {                                                                       \
        if(expression){}                                                    \
        else{                                                               \
            report_assertion_failure(#expression, "", __FILE__, __LINE__);  \
            debugBreak();                                                   \
        }                                                                   \
    }

#define ERI_ASSERT_MESSAGE(expression, message)                                     \
    {                                                                               \
        if(expression){}                                                            \
        else{                                                                       \
            report_assertion_failure(#expression, message, __FILE__, __LINE__);     \
            debugBreak();                                                           \
        }                                                                           \
    }

#ifdef _DEBUG
#define ERI_ASSERT_DEBUG(expression)                                                \
    {                                                                               \
        if(expression){}                                                            \
        else{                                                                       \
            report_assertion_failure(#expression, "", __FILE__, __LINE__);          \
            debugBreak();                                                           \
        }                                                                           \
    }
#else 
#define ERI_ASSERT_DEBUG(expression)
#endif

#else
#define ERI_ASSERT(expression)
#define ERI_ASSERT_MESSAGE(expression, message)
#define ERI_ASSERT_DEBUG(expression)
#endif