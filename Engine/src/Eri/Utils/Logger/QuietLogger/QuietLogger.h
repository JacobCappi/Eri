#pragma once

#include "Eri/Core.h"
#include "Eri/Utils/ILogger.h"

namespace ERI
{
class ERI_API QuietLogger : public ILogger
{
public:
    QuietLogger() {};
    ~QuietLogger() {};

public: // IUtil
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

