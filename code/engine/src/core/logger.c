#include "core/asserts.h"

#include "core/logger.h"
#include "platform/platform.h"
#include "core/mem.h"

//TODO: Platform layor to fix this
#include <stdio.h>
#include <stdarg.h>

b8 init_logging(void)
{
    // TODO: Implement (maybe file i/o?)
    // TODO: Some kind of circular array for multiple logs

    // Assuming Init's completion before this log...
    ERI_LOG_INFO("Initializing Subsystem: [ Logger ]");
    return TRUE;
}

void shutdown_logging(void)
{
    ERI_LOG_INFO("Shutting Down Subsystem: [ Logger ]");
    //TODO: Implement
    // Idea is to store in query and push a lot of messages
    // in one i/o. If shutdown, make sure all logs are sent
}

void log_message(enum log_level level, const char *message, ...)
{
    u16 max_msg_sz = 32000;
    const char *logging_level[6] = {
        " [ FTL ] ",
        " [ ERR ] ",
        " [ WRN ] ",
        " [ INF ] ",
        " [ DBG ] ",
        " [ TRC ] "
    };

    char input_message[max_msg_sz];
    char output_message[max_msg_sz];
    eri_memset(input_message, 0, sizeof(input_message));

    // apparently Microsoft and Clang don't agree on stuff, so this solves a bug
    // over the traditional va_list
    __builtin_va_list arg_ptr;
    va_start(arg_ptr, message);
    vsnprintf(input_message, sizeof(input_message), message, arg_ptr);
    va_end(arg_ptr);

    snprintf(output_message, max_msg_sz, "%s%s\n", logging_level[level], input_message);

    // Fatals and Errors are given special treatment
    if (level < LOG_LEVEL_WARNING)
    {
        platform_cerr(output_message, level);
    }
    else
    {
        platform_cout(output_message, level);
    }

}

void report_assertion_failure(const char* bool_expression, const char* message, const char* origin_file, i32 line_number)
{
    log_message(LOG_LEVEL_FATAL, "Failed Assertion: %s\n\tMessage: %s\n\tIn File: %s\n\t\tLine: %d\n", bool_expression, message, origin_file, line_number);
}