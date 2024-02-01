#include "Subsystems/ISubsystems.h"
#include "Subsystems/Subsystems.h"
#include "Subsystems/Factory.h"

#include "Subsystems/Logger/BasicLogger/BasicLogger.h"
#include "Subsystems/Platform/Windows/PlatformWindows.h"
#include "Subsystems/Events/VectorEvents/VectorEvents.h"
#include "Subsystems/Renderer/Vulkan/VulkanRenderer.h"

namespace ERI
{

Factory::Factory(i32 x, i32 y, i32 w, i32 h, const char *name)
{
    _x = x;
    _y = y;
    _width = w;
    _height = h;

    _app_name = name;
}

bool Factory::BuildLogger()
{
    _log = new BasicLogger();
    _log->SetLogLevel(false);


    _subsystems.push_back(_log);
    return true;
}

bool Factory::BuildEventSystem()
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

bool Factory::BuildPlatform()
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

bool Factory::BuildRenderer()
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

bool Factory::Startup()
{
    bool retVal = true;

    _log = nullptr;
    _events = nullptr;
    _platform = nullptr;
    _subsystems.clear();

    bool isLog = BuildLogger();
    bool isEvents = BuildEventSystem();
    bool isPlatform = BuildPlatform();
    bool isRenderer = BuildRenderer();

    for (auto system : _subsystems)
    {
        retVal = retVal && system->Startup();
    }

    _platform->SetWindowPosition(_x, _y);
    _platform->SetWindowSize(_width, _height);
    _platform->StartupWindow(_app_name);

    _renderer->setAppName(_app_name);

    return isLog && isEvents && isPlatform && isRenderer && retVal;
}

bool Factory::Shutdown()
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


ILogger* Factory::getLogger()
{
    return _log;
}

IEvents* Factory::getEventSystem()
{
    return _events;

}

IPlatform* Factory::getPlatform()
{
    return _platform;
}

IRenderer* Factory::getRenderer()
{
    return _renderer;
}

} // namespace ERI


