#pragma once
#include "defines.h"

#define LOG_WARNING_ENABLED     1
#define ERI_LOG_INFO_ENABLED    1
#define LOG_DEBUG_ENABLED       1
#define LOG_TRACE_ENABLED       1

#if RELEASE == 1
# define LOG_DEBUG_ENABLE = 0
# define LOG_TRACE_ENABLE = 0
#endif

typedef enum log_level{
    LOG_LEVEL_FATAL     = 0,
    LOG_LEVEL_ERROR     = 1,
    LOG_LEVEL_WARNING   = 2,
    LOG_LEVEL_INFO      = 3,
    LOG_LEVEL_DEBUG     = 4,
    LOG_LEVEL_TRACE     = 5
} log_level;

// ----- Subsystem Handling
b8 init_logging(void);
void shutdown_logging(void);
// ------ END


ERI_API void log_message(log_level level, const char *message, ...);

// These are always enabled
#ifndef ERI_LOG_FATAL
    #define ERI_LOG_FATAL(message, ...) log_message(LOG_LEVEL_FATAL, message, ##__VA_ARGS__)
#endif

#ifndef ERI_LOG_ERROR
    #define ERI_LOG_ERROR(message, ...) log_message(LOG_LEVEL_ERROR, message, ##__VA_ARGS__)
#endif

#if LOG_WARNING_ENABLED == 1
    #define ERI_LOG_WARNING(message, ...) log_message(LOG_LEVEL_WARNING, message, ##__VA_ARGS__)
#else
    #define ERI_LOG_WARNING(message, ...) 
#endif

#if ERI_LOG_INFO_ENABLED == 1
    #define ERI_LOG_INFO(message, ...) log_message(LOG_LEVEL_INFO, message, ##__VA_ARGS__)
#else
    #define ERI_LOG_INFO(message, ...) 
#endif

#if LOG_DEBUG_ENABLED == 1
    #define ERI_LOG_DEBUG(message, ...) log_message(LOG_LEVEL_DEBUG, message, ##__VA_ARGS__)
#else
    #define ERI_LOG_DEBUG(message, ...) 
#endif

#if LOG_TRACE_ENABLED == 1
    #define ERI_LOG_TRACE(message, ...) log_message(LOG_LEVEL_TRACE, message, ##__VA_ARGS__)
#else
    #define ERI_LOG_TRACE(message, ...) 
#endif