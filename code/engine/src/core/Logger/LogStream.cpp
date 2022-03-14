#include "LogStream.h"

// TODO: figure out streams ...
// Custom string lib sounded duable in C...
// This stream manip stuff --- how portable is it?
#include <iostream>

namespace ERI
{

    LogStream::LogStream(const Logger &logger, enum log_levels level)
    {
        // TODO: Memory management system
        stream = new(std::stringstream);
        internal_level = level;
    }

    LogStream::~LogStream()
    {
        std::cout << get_level(internal_level) << stream->str() << "\033[0m" << std::endl;
        delete stream;
    }

    const char* LogStream::get_level(enum log_levels level)
    {
        switch (level)
        {
            case LOG_LEVEL_FATAL:
                return "\033[0;41m [ FTL ] ";
            case LOG_LEVEL_ERROR:
                return "\033[1;31m [ ERR ] ";
            case LOG_LEVEL_WARN:
                return "\033[1;33m [ WRN ] ";
            case LOG_LEVEL_INFO:
                return "\033[1;32m [ INF ] ";
            case LOG_LEVEL_DEBUG:
                return "\033[1;34m [ DBG ] ";
            case LOG_LEVEL_TRACE:
                return "\033[1:30m [ TRC ] ";
            default:
                return " [ N/A ] ";
                break;
        }
    }
}