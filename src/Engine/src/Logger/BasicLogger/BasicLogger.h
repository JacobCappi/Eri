#pragma once

#include "Core.h"
#include "Logger/ILogger.h"

namespace ERI
{
class ERI_API BasicLogger : public ILogger
{
private:
    const unsigned int _maxMessageSize = 10000;
    bool _isRelease;

    enum LOG_TYPE
    {
        ERROR,
        WARNING,
        INFO,
        DEBUG,
        TRACE
    };
    const char *_colorStrings[5] = {"1;31", "1;33", "1;32", "1;34", "1;30"};

    const char *_types[5] = 
    {
        "[ ERR ] : ",
        "[ WRN ] : ",
        "[ INF ] : ",
        "[ DBG ] : ",
        "[ TRC ] : "
    };

public:
    BasicLogger() {};
    ~BasicLogger() {};

public: // ILogger
    void LogError(const char *, ...) override;
    void LogWarning(const char *, ...) override;
    void LogInfo(const char *, ...) override;
    void LogDebug(const char *, ...) override;
    void LogTrace(const char *, ...) override;

    void SetLogLevel(bool isRelease) override;

    void Startup() override;
    void Shutdown() override;
};
    
} // namespace ERI

