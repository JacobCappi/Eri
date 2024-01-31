#pragma once
#include "Subsystems/Subsystems.h"
#include "vector"

namespace ERI
{
class Factory : public ISubsystems
{
private:
    ILogger *_log;
    IPlatform *_platform;
    IEvents *_events;

    std::vector<ISubsystems *> _subsystems;

private:
    bool BuildLogger();
    bool BuildEventSystem();
    bool BuildPlatform();

public:
    Factory() {};
    ~Factory() {};

public: // ISubsystem
    bool Startup() override;
    bool Shutdown() override;

public:
    ILogger *getLogger();
    IEvents *getEventSystem();
    IPlatform *getPlatform();
};
} // namespace ERI
