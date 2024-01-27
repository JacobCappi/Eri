#include "Subsystems/Logger/QuietLogger/QuietLogger.h"

#include <stdio.h>
#include <stdarg.h>

namespace ERI
{

bool QuietLogger::Startup()
{
    return true;
}

bool QuietLogger::Shutdown()
{
    return true;
}

void QuietLogger::LogInfo(const char *buffer, ...)
{
}

void QuietLogger::LogDebug(const char *buffer, ...)
{
}

void QuietLogger::LogTrace(const char *buffer, ...)
{
}

void QuietLogger::LogWarning(const char *buffer, ...)
{
}

void QuietLogger::LogError(const char *buffer, ...)
{
}


void QuietLogger::SetLogLevel(bool isRelease)
{
}


} // namespace ERI
