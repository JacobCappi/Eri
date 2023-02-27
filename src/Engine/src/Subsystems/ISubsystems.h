#pragma once
#include "Logger/ILogger.h"

namespace ERI
{
    
class ISubsystems
{
public:
    ISubsystems() {};
    virtual ~ISubsystems() {};

public:
    virtual void Startup(ILogger) = 0;
    virtual void Shutdown() = 0;
};

} // namespace ERI