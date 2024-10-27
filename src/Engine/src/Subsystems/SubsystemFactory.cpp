#include "Subsystems/ISubsystems.h"
#include "Subsystems/Subsystems.h"
#include "Subsystems/SubsystemFactory.h"

#include "Subsystems/Logger/BasicLogger/BasicLogger.h"
#include "Subsystems/Platform/Windows/PlatformWindows.h"
#include "Subsystems/Events/VectorEvents/VectorEvents.h"
#include "Subsystems/Renderer/Vulkan/VulkanRenderer.h"

namespace ERI
{

bool SubsystemFactory::BuildLogger()
{
    _log = new BasicLogger();
    _log->SetLogLevel(false);

    _subsystems.push_back(_log);
    return true;
}

bool SubsystemFactory::BuildEventSystem()
{
    if (_log == nullptr)
    {
        return false;
    }
    _events = new VectorEvents();
    _events->RegisterLogger(_log);
    _subsystems.push_back(_events);
    return true;
}

bool SubsystemFactory::BuildPlatform()
{
    if (_log == nullptr || _events == nullptr)
    {
        return false;
    }

    // TODO: no other platform support yet
    if (!ERI_WINDOWS)
    {
        return false;
    }

    _log->LogInfo("Detected Platform Windows");
    _platform = new PlatformWindows();
    _platform->registerLogger(_log);
    _platform->registerEvents(_events);

    _subsystems.push_back(_platform);
    return true;
}

bool SubsystemFactory::BuildRenderer()
{
    if (_log == nullptr || _events == nullptr || _platform == nullptr)
    {
        return false;
    }

    _renderer = new VulkanRenderer();
    _renderer->registerLogger(_log);
    _renderer->registerEvents(_events);
    _renderer->registerPlatform(_platform);

    _subsystems.push_back(_renderer);
    return true;

}

bool SubsystemFactory::Startup()
{
    bool retVal = true;

    _log = nullptr;
    _events = nullptr;
    _platform = nullptr;
    _subsystems.clear();

    if (!BuildLogger())
    {
        return false;
    }

    if (!BuildEventSystem())
    {
        return false;
    }

    if (!BuildPlatform())
    {
        return false;
    }

    if (!BuildRenderer())
    {
        return false;
    }

    for (auto system : _subsystems)
    {
        retVal &= system->Startup();
    }

    _platform->SetWindowPosition(50, 50);
    _platform->SetWindowSize(800, 600);
    _platform->StartupWindow("ERI Game Engine - Windows");

    return retVal;
}

bool SubsystemFactory::Shutdown()
{
    bool retVal = true;
    for (auto system : _subsystems)
    {
        retVal = retVal && system->Shutdown();
    }
    _subsystems.clear();

    delete _log;
    delete _events;
    delete _platform;

    _log = nullptr;
    _events = nullptr;
    _platform = nullptr;

    return retVal;
}


ILogger* SubsystemFactory::getLogger()
{
    return _log;
}

IEvents* SubsystemFactory::getEventSystem()
{
    return _events;

}

IPlatform* SubsystemFactory::getPlatform()
{
    return _platform;
}

IRenderer* SubsystemFactory::getRenderer()
{
    return _renderer;
}

} // namespace ERI


