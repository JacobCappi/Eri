#pragma once
#include "defines.h"

#include "LogStream.h"

// https://www.geeksforgeeks.org/c-stream-classes-structure/

namespace ERI
{
    class Logger
    {
        friend class LogStream;

    public:
        explicit Logger();
        ~Logger();

    public:
        LogStream log_fatal() const;
        LogStream log_error() const;
        LogStream log_warn() const;
        LogStream log_info() const;
        LogStream log_debug() const;
        LogStream log_trace() const;
    };

}