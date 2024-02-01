#pragma once

#include "Subsystems/ISubsystems.h"
#include "Subsystems/Logger/ILogger.h"
#include "Subsystems/Events/IEvents.h"


namespace ERI
{

class IRenderer : public ISubsystems
{
public:
   IRenderer() {};
   virtual ~IRenderer() {};

public:
   virtual void registerLogger(ILogger *log) = 0;
   virtual void registerEvents(IEvents *events) = 0;
   virtual void registerPlatform(IPlatform *platform) = 0;
   virtual void setAppName(const char *app_name) = 0;
};

} // namespace ERI


