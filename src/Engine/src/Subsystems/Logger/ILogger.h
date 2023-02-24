#include "Subsystems/ISubsystems.h"

namespace ERI
{


class ILogger : public ISubsystems
{

public:
    ILogger() {};
    virtual ~ILogger() {};

public:
    // TODO: get from Core from compiler flag
public:
    virtual void LogInfo(const char *, ...) = 0;
    virtual void LogDebug(const char *, ...) = 0;
    virtual void LogTrace(const char *, ...) = 0;
    virtual void LogWarning(const char *, ...) = 0;
    virtual void LogError(const char *, ...) = 0;

    virtual void SetLogLevel(bool isRelease) = 0;
};
    
} // namespace ERI