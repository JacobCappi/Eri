#pragma once

#define ENGINE_NAME "ERI_ENGINE"

/* Unsigned int types */
typedef unsigned long long int u64;
typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;

/* Signed int types */
typedef signed long long int i64;
typedef signed int i32;
typedef signed short i16;
typedef signed char i8;

/* Floating Point types */
typedef double f64;
typedef float f32;

#if defined(__clang__) || defined(__gcc__)
  #define STATIC_ASSERT _Static_assert
#else
  #define STATIC_ASSERT static_assert
#endif

/* Ensures the types are the correct size */
STATIC_ASSERT(sizeof(u64) == 8, "Expected u64 to be 8 bytes.");
STATIC_ASSERT(sizeof(u32) == 4, "Expected u32 to be 4 bytes.");
STATIC_ASSERT(sizeof(u16) == 2, "Expected u16 to be 2 bytes.");
STATIC_ASSERT(sizeof(u8) == 1, "Expected u8 to be 1 byte.");

STATIC_ASSERT(sizeof(i64) == 8, "Expected i64 to be 8 bytes.");
STATIC_ASSERT(sizeof(i32) == 4, "Expected i32 to be 4 bytes.");
STATIC_ASSERT(sizeof(i16) == 2, "Expected i16 to be 2 bytes.");
STATIC_ASSERT(sizeof(i8) == 1, "Expected i8 to be 1 byte.");

STATIC_ASSERT(sizeof(f64) == 8, "Expected f64 to be 8 bytes.");
STATIC_ASSERT(sizeof(f32) == 4, "Expected f32 to be 4 bytes.");

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
  #define ERI_WINDOWS 1
  #ifndef _WIN64
    #error "32 bit windows is not supported"
  #endif
#elif defined(__linux__) || defined(__gnu_linux__)
  #define ERI_LINUX 1
  #if defined(__ANDROID___)
    #define ERI_ANDROID 1
    #error "Android not supported"
  #endif
#endif

#ifdef ERI_EXPORT
  #if ERI_WINDOWS == 1
    #define ERI_API __declspec(dllexport)
  #else
    #define ERI_API __attribute__((visibility("default")))
  #endif
#else
  #if ERI_WINDOWS == 1
    #define ERI_API __declspec(dllimport)
  #else
    #define ERI_API
  #endif
#endif

#ifdef _DEBUG
  #define ERI_DEBUG 1
#else
  #define ERI_RELEASE 1
#endif