#pragma once
#include "Eri/Core.h"

#include <vulkan/vulkan.h>

#include "Eri/Renderer/IRenderer.h"

#include "Eri/Utils/EriUtils.h"
#include "Eri/Platform/EriPlatform.h"

#include "Eri/Utils/IUtil.h"
#include "Eri/Utils/ILogger.h"
#include "Eri/Utils/IEvents.h"

namespace ERI
{

class VulkanRenderer : public IRenderer
{
private:
  ILogger *_log;
  IEvents *_events;
  IPlatform *_platform;

  Platforms _platform_type;

  const char *_app_name = nullptr;

private:
  VkInstance _vulkan_instance;

public:
  VulkanRenderer() {};
  ~VulkanRenderer() {};

public: // IRenderer
  void registerUtils(EriUtils *utils) override;
  void registerPlatform(EriPlatform *platform) override;
  void setAppName(const char *app_name) override;

  bool Startup() override;
  bool Shutdown() override;


};
  
} // namespace ERI
