#pragma once
#include "defines.h"

#define LOG_WARNING_ENABLED 1
#define LOG_INFO_ENABLED 1
#define LOG_DEBUG_ENABLED 1
#define LOG_TRACE_ENABLED 1

#if RELEASE == 1
# define LOG_DEBUG_ENABLE = 0
# define LOG_TRACE_ENABLE = 0
#endif

#define COLOR_ERROR "\x1b[31m"
#define COLOR_WARNING "\x1b[33m"
#define COLOR_RESET "\x1b[0m"

typedef enum log_level{
    LOG_LEVEL_FATAL = 0,
    LOG_LEVEL_ERROR = 1,
    LOG_LEVEL_WARNING = 2,
    LOG_LEVEL_INFO = 3,
    LOG_LEVEL_DEBUG = 4,
    LOG_LEVEL_TRACE = 5
} log_level;

// ----- START function def
b8 init_logging();
void shutdown_logging();
ERI_API void log_message(log_level level, const char *message, ...);
// ------ END


// These are always enabled
#ifndef LOG_FATAL
    #define LOG_FATAL(message, ...) log_message(LOG_LEVEL_FATAL, message, ##__VA_ARGS__)
#endif

#ifndef LOG_ERROR
    #define LOG_ERROR(message, ...) log_message(LOG_LEVEL_ERROR, message, ##__VA_ARGS__)
#endif

// KOHI's idea of not defining if not enabled. Apparently more effecient
#if LOG_WARNING_ENABLED == 1
    #define LOG_WARNING(message, ...) log_message(LOG_LEVEL_WARNING, message, ##__VA_ARGS__)
#else
    #define LOG_WARNING(message, ...) 
#endif

#if LOG_INFO_ENABLE == 1
    #define LOG_INFO(message, ...) log_message(LOG_LEVEL_WARNING, message, ##__VA_ARGS__)
#else
    #define LOG_INFO(message, ...) 
#endif

#if LOG_DEBUG_ENABLED == 1
    #define LOG_DEBUG(message, ...) log_message(LOG_LEVEL_WARNING, message, ##__VA_ARGS__)
#else
    #define LOG_DEBUG(message, ...) 
#endif

#if LOG_TRACE_ENABLED == 1
    #define LOG_TRACE(message, ...) log_message(LOG_LEVEL_WARNING, message, ##__VA_ARGS__)
#else
    #define LOG_TRACE(message, ...) 
#endif