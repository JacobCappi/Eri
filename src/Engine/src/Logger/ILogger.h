#pragma once

namespace ERI
{


class ILogger
{

public:
    ILogger() {};
    virtual ~ILogger() {};

public:
// Similiar interface to subsystem, but subsystem should include logger.
// thus Logger is not a subsystem, but some loggers (file i/o) need these 2 functions
    virtual void Startup() = 0;
    virtual void Shutdown() = 0;

public:
    virtual void LogInfo(const char *, ...) = 0;
    virtual void LogDebug(const char *, ...) = 0;
    virtual void LogTrace(const char *, ...) = 0;
    virtual void LogWarning(const char *, ...) = 0;
    virtual void LogError(const char *, ...) = 0;

    virtual void SetLogLevel(bool isRelease) = 0;
};
    
} // namespace ERI