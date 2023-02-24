#pragma once
#include "Subsystems/ISubsystems.h"
#include "Subsystems/Logger/ILogger.h"

namespace ERI
{

class IEventSystem : public ISubsystems
{
private:
    ILogger *_log;
public:
    IEventSystem(ILogger *log);
};
    
} // namespace ERI