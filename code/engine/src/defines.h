#pragma once

// ----- Start types
/* Unsigned int types */
typedef unsigned long long u64;
typedef unsigned int       u32;
typedef unsigned short     u16;
typedef unsigned char       u8;

/* Signed int types */
typedef signed long long   i64;
typedef signed int         i32;
typedef signed short       i16;
typedef signed char         i8;

/* Floating Point types */
typedef double             f64;
typedef float              f32;

/* Boolean types */
typedef int                b32; 
typedef char                b8;

#define TRUE 1
#define FALSE 0
// ----- END

/* Making sure asserts are properly defined */
#if defined(__clang__) || defined(__gcc__)
#define STATIC_ASSERT _Static_assert
#else
#define STATIC_ASSERT static_assert
#endif

// ----- Start checking all typedefs 
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
// ----- END

// ----- Start Platform Detection
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) 
#define ERI_PLATFORM_WINDOWS 1
#ifndef _WIN64
#error "[ ERR ] 32 bit Windows not supported"
#endif
#elif defined(__linux__) || defined(__gnu_linux__)
#define ERI_PLATFORM_LINUX 1
#if defined(__ANDROID__)
#define ERI_PLATFORM_ANDROID 1
#endif
#elif defined(__unix__)
#define ERI_PLATFORM_UNIX 1
#elif defined(_POSIX_VERSION)
#define ERI_PLATFORM_POSIX 1
#elif __APPLE__
#define ERI_PLATFORM_APPLE 1
#include <TargetConditionals.h>
#if TARGET_IPHONE_SIMULATOR
#define ERI_PLATFORM_IOS 1
#define ERI_PLATFORM_IOS_SIMULATOR 1
#elif TARGET_OS_IPHONE
#define ERI_PLATFORM_IOS 1
#elif TARGET_OS_MAC
#else
#error "Unknown Apple platform"
#endif
#else
#error "Unknown platform!"
#endif
// ----- END

// ----- Start Declspec definitions
#ifdef ERI_EXPORT
#ifdef _MSC_VER
#define ERI_API __declspec(dllexport)
#else
#define ERI_API __attribute__((visibility("default")))
#endif
#else
#ifdef _MSC_VER
#define ERI_API __declspec(dllimport)
#else
#define ERI_API
#endif
#endif
// ----- END


