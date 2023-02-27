#pragma once

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