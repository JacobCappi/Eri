#include "Eri/Utils/Logger/QuietLogger/QuietLogger.h"

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
