#pragma once
#include "defines.h"

#include <sstream>

// http://www.cplusplus.com/forum/general/223039/

namespace ERI
{
    // forward declare
    class Logger;

    class LogStream
    {
        friend class Logger;

    private:
        enum log_levels
        {
            LOG_LEVEL_FATAL,
            LOG_LEVEL_ERROR,
            LOG_LEVEL_WARN,
            LOG_LEVEL_INFO,
            LOG_LEVEL_DEBUG,
            LOG_LEVEL_TRACE
        };

    private:
        std::stringstream *stream;
        enum log_levels internal_level;

    private:
        LogStream(const Logger &logger, enum log_levels);
        static const char* get_level(enum log_levels level);

    public:
        ~LogStream();

        template <class T>
        LogStream& operator << (const T& stream_value) 
        {
            if (stream != nullptr)
            {
                *stream << stream_value;
            }
            return (*this);
        }

    };

}