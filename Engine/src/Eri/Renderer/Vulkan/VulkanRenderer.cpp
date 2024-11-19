#include "Eri/Renderer/Vulkan/VulkanRenderer.h"

#include <vulkan/vulkan_core.h>

#include "Eri/Utils/EriUtils.h"
#include "Eri/Platform/EriPlatform.h"

#include "Eri/Utils/IUtil.h"
#include "Eri/Utils/ILogger.h"
#include "Eri/Utils/IEvents.h"

namespace ERI
{

bool VulkanRenderer::Startup()
{
  if (_log == nullptr || _events == nullptr || _app_name == nullptr)
  {
    return false;
  }

  VkApplicationInfo appInfo = {};
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = _app_name;
  appInfo.applicationVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
  appInfo.engineVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
  appInfo.apiVersion = VK_API_VERSION_1_0;
  appInfo.pEngineName = ENGINE_NAME;

  VkInstanceCreateInfo createInfo = {};
  createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pApplicationInfo = &appInfo;
  // TODO:

  void *window = 0;
  const void *display = nullptr;
  _platform->getWindowPointers(window, &display);

  const u32 max_ext = 2;
  const char *extensions[max_ext];
  _platform->getWindowExtention(extensions);
  extensions[1] = VK_KHR_SURFACE_EXTENSION_NAME;

  createInfo.enabledExtensionCount = max_ext;
  createInfo.ppEnabledExtensionNames = extensions;

  createInfo.enabledLayerCount = 0;

  VkResult res = vkCreateInstance(&createInfo, nullptr, &_vulkan_instance);

  if (res != VkResult::VK_SUCCESS)
  {
    _log->LogError("Vulkan failed to create instance with error code %zu", res);
    return false;
  }

  return true;
}

bool VulkanRenderer::Shutdown()
{
  vkDestroyInstance(_vulkan_instance, nullptr);
  return true;
}

void VulkanRenderer::registerUtils(EriUtils *utils)
{
  _log = utils->getLogger();
  _events = utils->getEventSystem();
}

void VulkanRenderer::registerPlatform(EriPlatform *platform)
{
  _platform = platform->getEriPlatform();
  _platform_type = platform->platformType();
}

void VulkanRenderer::setAppName(const char *app_name)
{
  _app_name = app_name;
}


} // namespace ERI

