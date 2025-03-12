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

  if (!startupValidationLayers())
  {
    return false;
  }

  if (!startupPhysicalDevice())
  {
    return false;
  }

  return true;
}

bool VulkanRenderer::Shutdown()
{
  destroyDebugCallback();
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

bool VulkanRenderer::startupValidationLayers()
{
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
  enableDebugCallback();

  return true;

}

bool VulkanRenderer::startupPhysicalDevice()
{

}

bool VulkanRenderer::enableExtentionSupport(VkInstanceCreateInfo &createInfo)
{
  _platform->getWindowExtention(_extensions);
  _extensions[1] = VK_KHR_SURFACE_EXTENSION_NAME;
  _extensions[2] = VK_EXT_DEBUG_UTILS_EXTENSION_NAME;

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

  bool are_any_validation_layers_enabled = false;

  for (const auto &requestedLayer : _validationLayer)
  {
    bool is_validation_layer_supported = false;

    for (const auto &layerProps : layersAvailable)
    {
      if (strcmp(requestedLayer, layerProps.layerName) == 0)
      {
        is_validation_layer_supported = true;
        are_any_validation_layers_enabled = true;
        break;
      }
    }

    if (!is_validation_layer_supported)
    {
      _log->LogDebug("Unable to support Vulkan validation layer %s. Disabled", requestedLayer);
    }
  }

  if (!are_any_validation_layers_enabled)
  {
    _log->LogDebug("No validation layers enabled for this build");
    createInfo.enabledLayerCount = 0;
    return false;
  }

  createInfo.enabledLayerCount = max_layers;
  createInfo.ppEnabledLayerNames = _validationLayer;
  return true;
}

bool VulkanRenderer::enableDebugCallback()
{
  if (!_enableValidationLayers)
  {
    _log->LogDebug("Validiation layer disabled, debug callback not supported");
    return false;
  }

  // Extension functions require you to go fetch the address of the function
  // as they are not automatically loaded:
  // https://vulkan-tutorial.com/Drawing_a_triangle/Setup/Validation_layers
  auto createDebugUtilsFunc = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(_vulkan_instance, "vkCreateDebugUtilsMessengerEXT");
  if (createDebugUtilsFunc == nullptr)
  {
    _log->LogWarning("Validation Layer was enabled, but failed to bind debug function. Questionable state?");
    return false;
  }

  VkDebugUtilsMessengerCreateInfoEXT createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;

  createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                               VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                               VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
                               VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;

  createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                           VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                           VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

  createInfo.pfnUserCallback = vulkanDebugCallback;
  createInfo.pUserData = this->_log; // optional

  if (createDebugUtilsFunc(_vulkan_instance, &createInfo, nullptr, &_vulkan_debug_messenger) != VkResult::VK_SUCCESS)
  {
    _log->LogError("Vulkan debug utils function was unable to be allocated. Unsure why?");
    return false;
  }

  return true;
}

bool VulkanRenderer::destroyDebugCallback()
{
  if (!_enableValidationLayers)
  {
    _log->LogDebug("Validiation layer disabled, debug callback cannot be deallocated");
    return false;
  }

  auto destroyDebugUtilsFunc = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(_vulkan_instance, "vkDestroyDebugUtilsMessengerEXT");
  if (destroyDebugUtilsFunc == nullptr)
  {
    _log->LogWarning("Vulkan was unable find the deallocate function for the debug messenger utility. Unsure why?");
    return false;
  }

  destroyDebugUtilsFunc(_vulkan_instance, _vulkan_debug_messenger, nullptr);
  return true;
}


VKAPI_ATTR VkBool32 VKAPI_CALL VulkanRenderer::vulkanDebugCallback(
  VkDebugUtilsMessageSeverityFlagBitsEXT severity,
  VkDebugUtilsMessageTypeFlagsEXT type,
  const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
  void* pContext)
{
  ILogger *log = (ILogger*)pContext;

  log->LogDebug("%s", pCallbackData->pMessage);

  /*
  The callback returns a boolean that indicates if the Vulkan call that triggered the
  validation layer message should be aborted. If the callback returns true, then the
  call is aborted with the VK_ERROR_VALIDATION_FAILED_EXT error. This is normally only
  used to test the validation layers themselves, so you should always return VK_FALSE.
  */
  return VK_FALSE;
}



} // namespace ERI
