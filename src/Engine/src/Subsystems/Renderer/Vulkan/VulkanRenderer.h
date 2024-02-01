#pragma once

#include "Subsystems/Subsystems.h"
#include "Subsystems/Renderer/IRenderer.h"


namespace ERI
{

class VulkanRenderer : public IRenderer
{
private:
   const char *_app_name;

private:
   ILogger *_log;
   IEvents *_events;
   IPlatform *_platform;

public:
   VulkanRenderer() {};
   ~VulkanRenderer() {};

public: // ISubsystems
   bool Startup() override;
   bool Shutdown() override;

public: // IVulkanRenderer
   void registerLogger(ILogger *log) override;
   void registerEvents(IEvents *events) override;
   void registerPlatform(IPlatform *platform) override;
   void setAppName(const char *app_name) override;
};

} // namespace ERI

