#include "logger.h"
#include "asserts.h"

//TODO: Platform layor to fix this
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

b8 init_logging()
{
    // TODO: Implement
    return TRUE;
}

void shutdown_logging()
{
    //TODO: Implement
    // Idea is to store in query and push a lot of messages
    // in one i/o. If shutdown, make sure all logs are sent
}

void log_message(log_level level, const char *message, ...)
{
    const char *logging_level[6] = {
        " FATAL | ",
        " ERROR | ",
        " WARN  | ",
        " INFO  | ",
        " DEBUG | ",
        " TRACE | "
    };

    // Input message is apparently not dynamic as it is 'slower'. Chose to put it on stack
    // Poses 32k char limit though (probably not a problem)
    char input_message[32000];
    char output_message[32000];
    memset(input_message, 0, sizeof(input_message));

    // apparently Microsoft and Clang don't agree on stuff, so this solves a bug
    // over the traditional va_list
    __builtin_va_list arg_ptr;
    va_start(arg_ptr, message);
    vsnprintf(input_message, sizeof(input_message), message, arg_ptr);
    va_end(arg_ptr);

    sprintf(output_message, "%s%s\n", logging_level[level], input_message);
    // TODO: application layor specific output 
    printf("%s", output_message);

}

void report_assertion_failure(const char* bool_expression, const char* message, const char* origin_file, i32 line_number)
{
    log_message(LOG_LEVEL_FATAL, "Failed Assertion: %s\n\tMessage: %s\n\tIn File: %s\n\t\tLine: %d\n", bool_expression, message, origin_file, line_number);
}