#include "Logger.h"

namespace ERI
{
    Logger::Logger()
    {
    }
    
    Logger::~Logger()
    {
    }

    LogStream Logger::log_fatal() const
    {
        return LogStream(*this, LogStream::LOG_LEVEL_FATAL);
    }

    LogStream Logger::log_error() const
    {
        return LogStream(*this, LogStream::LOG_LEVEL_ERROR);
    }

    LogStream Logger::log_warn() const
    {
        return LogStream(*this, LogStream::LOG_LEVEL_WARN);
    }

    LogStream Logger::log_info() const
    {
        return LogStream(*this, LogStream::LOG_LEVEL_INFO);
    }

    LogStream Logger::log_debug() const
    {
        return LogStream(*this, LogStream::LOG_LEVEL_DEBUG);
    }

    LogStream Logger::log_trace() const
    {
        return LogStream(*this, LogStream::LOG_LEVEL_TRACE);
    }

}