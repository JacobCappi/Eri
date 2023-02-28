#include "Subsystems/Logger/BasicLogger/BasicLogger.h"

#include <stdio.h>
#include <stdarg.h>

namespace ERI
{

void BasicLogger::Startup()
{
    LogInfo("Logger is starting up");
}

void BasicLogger::Shutdown()
{
    LogInfo("Logger set for shutdown");
}

void BasicLogger::LogInfo(const char *buffer, ...)
{
    if (_isRelease)
    {
        return;
    }

    char inputBuffer[_maxMessageSize];

    __builtin_va_list argv;
    va_start(argv, buffer);
    vsnprintf(inputBuffer, _maxMessageSize, buffer, argv);
    va_end(argv);

    printf("\033[%sm%s%s\033[0m\n",_colorStrings[INFO], _types[INFO], inputBuffer);
}

void BasicLogger::LogDebug(const char *buffer, ...)
{
    if (_isRelease)
    {
        return;
    }

    char inputBuffer[_maxMessageSize];

    __builtin_va_list argv;
    va_start(argv, buffer);
    vsnprintf(inputBuffer, _maxMessageSize, buffer, argv);
    va_end(argv);

    printf("\033[%sm%s%s\033[0m\n",_colorStrings[DEBUG], _types[DEBUG], inputBuffer);
}

void BasicLogger::LogTrace(const char *buffer, ...)
{
    if (_isRelease)
    {
        return;
    }

    char inputBuffer[_maxMessageSize];

    __builtin_va_list argv;
    va_start(argv, buffer);
    vsnprintf(inputBuffer, _maxMessageSize, buffer, argv);
    va_end(argv);

    printf("\033[%sm%s%s\033[0m\n",_colorStrings[TRACE], _types[TRACE], inputBuffer);
}

void BasicLogger::LogWarning(const char *buffer, ...)
{
    char inputBuffer[_maxMessageSize];

    __builtin_va_list argv;
    va_start(argv, buffer);
    vsnprintf(inputBuffer, _maxMessageSize, buffer, argv);
    va_end(argv);

    printf("\033[%sm%s%s\033[0m\n",_colorStrings[WARNING], _types[WARNING], inputBuffer);
}

void BasicLogger::LogError(const char *buffer, ...)
{
    char inputBuffer[_maxMessageSize];

    __builtin_va_list argv;
    va_start(argv, buffer);
    vsnprintf(inputBuffer, _maxMessageSize, buffer, argv);
    va_end(argv);

    printf("\033[%sm%s%s\033[0m\n",_colorStrings[ERROR], _types[ERROR], inputBuffer);

}


void BasicLogger::SetLogLevel(bool isRelease)
{
    _isRelease = isRelease;
}


} // namespace ERI
