#include "Subsystems/Renderer/Vulkan/VulkanRenderer.h"
#include "Subsystems/Subsystems.h"

#ifdef ERI_WINDOWS
   #include <windows.h>
#elif ERI_LINUX
#endif

#include <vulkan/vulkan.h>



namespace ERI
{

void VulkanRenderer::registerLogger(ILogger *log)
{
   _log = log;
}

void VulkanRenderer::registerEvents(IEvents *events)
{
   _events = events;
}

void VulkanRenderer::registerPlatform(IPlatform *platform)
{
   _platform = platform;
}

void VulkanRenderer::setAppName(const char *app_name)
{
   _app_name = app_name;
}

bool VulkanRenderer::Startup()
{
   VkInstanceCreateInfo creation_info{};
   VkApplicationInfo app_info{};

// // TODO: abstract this later [everywhere a glfw call is used in the tutorial will need to be]
   app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
   app_info.pApplicationName = _app_name;
   app_info.applicationVersion = VK_MAKE_API_VERSION(0,1,0,0);
   app_info.pEngineName = ENGINE_NAME;
   app_info.engineVersion = VK_MAKE_API_VERSION(0,1,0,0);
   app_info.apiVersion = VK_API_VERSION_1_0;

   creation_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
   creation_info.pApplicationInfo = &app_info;


// // https://registry.khronos.org/vulkan/specs/1.0-wsi_extensions/pdf/vkspec.pdf
// // page 1510 is where I found windows stuff !_!
   return true;
}

bool VulkanRenderer::Shutdown()
{
   return false;

}

} // namespace ERI

