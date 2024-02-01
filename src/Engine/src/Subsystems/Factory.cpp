#include "Subsystems/ISubsystems.h"
#include "Subsystems/Subsystems.h"
#include "Subsystems/Factory.h"

#include "Subsystems/Logger/BasicLogger/BasicLogger.h"
#include "Subsystems/Platform/Windows/PlatformWindows.h"
#include "Subsystems/Events/VectorEvents/VectorEvents.h"

namespace ERI
{

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

    for (auto system : _subsystems)
    {
        retVal = retVal && system->Startup();
    }

    return isLog && isEvents && isPlatform && retVal;
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

    
} // namespace ERI


