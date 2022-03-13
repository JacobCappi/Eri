#include "LogStream.h"
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
        std::cout << get_level(internal_level) << stream->str() << std::endl;
        delete stream;
    }

    const char* LogStream::get_level(enum log_levels level)
    {
        switch (level)
        {
            case LOG_LEVEL_FATAL:
                return " [ FTL ] ";
            case LOG_LEVEL_ERROR:
                return " [ ERR ] ";
            case LOG_LEVEL_WARN:
                return " [ WRN ] ";
            case LOG_LEVEL_INFO:
                return " [ INF ] ";
            case LOG_LEVEL_DEBUG:
                return " [ DBG ] ";
            case LOG_LEVEL_TRACE:
                return " [ TRC ] ";
            default:
                return " [ N/A ] ";
                break;
        }
    }
}