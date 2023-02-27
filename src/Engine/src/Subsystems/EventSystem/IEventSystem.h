#pragma once
#include "Subsystems/ISubsystems.h"
#include "Subsystems/Logger/ILogger.h"

namespace ERI
{

class IEventSystem : public ISubsystems
{
public:
    IEventSystem();
    ~IEventSystem() {};

    virtual publishEvent()
};
    
} // namespace ERI