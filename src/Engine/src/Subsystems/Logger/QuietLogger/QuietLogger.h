#pragma once

#include "Core.h"
#include "Subsystems/Logger/ILogger.h"

namespace ERI
{
class ERI_API QuietLogger : public ILogger
{
public:
    QuietLogger() {};
    ~QuietLogger() {};

public: // ISubsystem
    bool Startup() override;
    bool Shutdown() override;

public: // ILogger
    void LogError(const char *, ...) override;
    void LogWarning(const char *, ...) override;
    void LogInfo(const char *, ...) override;
    void LogDebug(const char *, ...) override;
    void LogTrace(const char *, ...) override;

    void SetLogLevel(bool isRelease) override;
};
    
} // namespace ERI

