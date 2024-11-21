#include "Eri/Renderer/Vulkan/VulkanRenderer.h"

#include <vulkan/vulkan_core.h>

/* The goal is to do everything myself, makes these libs myself in util*/
#include <vector>
#include <cstring>

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

  if (!enableExtentionSupport(createInfo))
  {
    return false;
  }

  enableLayerSupport(createInfo);

  VkResult res = vkCreateInstance(&createInfo, nullptr, &_vulkan_instance);

  if (res != VkResult::VK_SUCCESS)
  {
    // TODO: If we ever support MAC, the tutorial talks bout a potential
    // missing driver to address
    _log->LogError("Vulkan failed to create instance with error code %zu", res);
    return false;
  }


  _log->LogInfo("Vulkan successfully created an instance");
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

bool VulkanRenderer::enableExtentionSupport(VkInstanceCreateInfo &createInfo)
{
  _platform->getWindowExtention(_extensions);
  _extensions[1] = VK_KHR_SURFACE_EXTENSION_NAME;

  u32 extCount = 0;
  vkEnumerateInstanceExtensionProperties(nullptr, &extCount, nullptr);

  std::vector<VkExtensionProperties> supportedExts(extCount);
  vkEnumerateInstanceExtensionProperties(nullptr, &extCount, supportedExts.data());

  for (const auto &exts : _extensions)
  {
    bool is_extensions_supported = false;
    for (const auto &supported : supportedExts)
    {
      if (strcmp(exts, supported.extensionName) == 0)
      {
        is_extensions_supported = true;
        break;
      }
    }

    if (!is_extensions_supported)
    {
      _log->LogError("Unable to find %s vulkan extension, creation failed", exts);
      return false;
    }
  }

  createInfo.enabledExtensionCount = _maxExtensions;
  createInfo.ppEnabledExtensionNames = _extensions;

  return true;
}

bool VulkanRenderer::enableLayerSupport(VkInstanceCreateInfo &createInfo)
{
  if (!_enableValidationLayers)
  {
    return false;
  }

  const u32 max_layers = 1;

  u32 layerCount;
  vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

  std::vector<VkLayerProperties> layersAvailable(layerCount);
  vkEnumerateInstanceLayerProperties(&layerCount, layersAvailable.data());

  bool is_validation_layer_supported = false;
  for (const auto &layerProps : layersAvailable)
  {
    if (strcmp(_validationLayer, layerProps.layerName) == 0)
    {
      is_validation_layer_supported = true;
      break;
    }
  }

  if (!is_validation_layer_supported)
  {
    _log->LogDebug("Unable to support Vulkan validation layers, they are disables");
    return false;
  }

  createInfo.enabledLayerCount = max_layers;
  createInfo.ppEnabledLayerNames = &_validationLayer;
  return true;
}


} // namespace ERI
