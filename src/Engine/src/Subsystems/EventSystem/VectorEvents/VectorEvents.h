#pragma once
#include "Subsystems/EventSystem/IEventSystem"
#include "Subsystems/ISubsystems.h"

namespace ERI
{
class VectorEvents : public IEventSystem
{

private:
    ILogger _log;

public:
    VectorEvents() {};
    ~VectorEvents() {};

public:

virtual void Startup() = 0;
virtual void Shutdown() = 0;


};
} // namespace ERI
