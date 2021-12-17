#pragma once
#include "defines.h"

// TODO: File i/o

#define LOG_WARNING_ENABLED     1
#define LOG_INFO_ENABLED        1
#define LOG_DEBUG_ENABLED       1
#define LOG_TRACE_ENABLED       1

#if RELEASE == 1
# define LOG_DEBUG_ENABLE = 0
# define LOG_TRACE_ENABLE = 0
#endif

enum log_level{
    LOG_LEVEL_FATAL,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_WARNING,
    LOG_LEVEL_INFO,
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_TRACE
};

// ----- Start Subsystem Handling 
b8   init_logging(void);
void shutdown_logging(void);
// ------ END


/* Part of the API to allow those defines to also be part of the API */
ERI_API void log_message(enum log_level level, const char *message, ...);

// ----- Start Logger Defines (these should be used to log things)

/* FATAL is always enabled */
#ifndef ERI_LOG_FATAL
    #define ERI_LOG_FATAL(message, ...) log_message(LOG_LEVEL_FATAL, message, ##__VA_ARGS__)
#endif

/* ERROR is always enabled */
#ifndef ERI_LOG_ERROR
    #define ERI_LOG_ERROR(message, ...) log_message(LOG_LEVEL_ERROR, message, ##__VA_ARGS__)
#endif

#if LOG_WARNING_ENABLED == 1
    #define ERI_LOG_WARNING(message, ...) log_message(LOG_LEVEL_WARNING, message, ##__VA_ARGS__)
#else
    #define ERI_LOG_WARNING(message, ...) 
#endif

#if LOG_INFO_ENABLED == 1
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
// ----- END