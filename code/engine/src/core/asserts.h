#pragma once
// NOTE: name is asserts*.h, as assert.h is a std lib header

#include "defines.h"

#define ERI_ASSERTIONS_ENABLED

#ifdef ERI_ASSERTIONS_ENABLED
// Also from KOHI, addressed bug with different naming schemes
#if _MSC_VER
    #include <intrin.h>
    #define debugBreak() __debugbreak()
#else
    #define debugBreak() __builtin_trap()
#endif

ERI_API void report_assertion_failure(const char* expression, const char* message, const char* file, i32 line);
