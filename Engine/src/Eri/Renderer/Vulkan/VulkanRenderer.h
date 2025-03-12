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

#ifdef ERI_DEBUG
  const bool _enableValidationLayers = true;
#else
  const bool enableValidationLayers = false;
#endif

  static const u32 _maxLayers = 1;
  const char *_validationLayer[_maxLayers] =
  {
    "VK_LAYER_KHRONOS_validation"
  };

  static const u32 _maxExtensions = 3;
  const char *_extensions[_maxExtensions];

private:
  VkInstance _vulkan_instance;
  VkDebugUtilsMessengerEXT _vulkan_debug_messenger;

public:
  VulkanRenderer() {};
  ~VulkanRenderer() {};

public: // IRenderer
  void registerUtils(EriUtils *utils) override;
  void registerPlatform(EriPlatform *platform) override;
  void setAppName(const char *app_name) override;

  bool Startup() override;
  bool Shutdown() override;

private: // Tutorial Breakdowns
  bool startupValidationLayers();
  bool startupPhysicalDevice();

private:
  bool enableExtentionSupport(VkInstanceCreateInfo &createInfo);
  bool enableLayerSupport(VkInstanceCreateInfo &createInfo);
  bool enableDebugCallback();
  bool destroyDebugCallback();

private:
  /*

    The messageType parameter can have the following values:

    VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT: Some event has happened that is unrelated to the specification or performance
    VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT: Something has happened that violates the specification or indicates a possible mistake
    VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT: Potential non-optimal use of Vulkan

    The pCallbackData parameter refers to a VkDebugUtilsMessengerCallbackDataEXT struct containing the details of the message itself, with the most important members being:

    pMessage: The debug message as a null-terminated string
    pObjects: Array of Vulkan object handles related to the message
    objectCount: Number of objects in array

   */
  static VKAPI_ATTR VkBool32 VKAPI_CALL vulkanDebugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT severity,
    VkDebugUtilsMessageTypeFlagsEXT type,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pContext);


};

} // namespace ERI
