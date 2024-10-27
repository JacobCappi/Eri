#pragma once
#include "Subsystems/Subsystems.h"
#include "vector"

namespace ERI
{
/**
 * @brief Constructs all of the subsystems within ERI
 *
 */
class SubsystemFactory : public ISubsystems
{
private:
    ILogger *_log;
    IPlatform *_platform;
    IEvents *_events;
    IRenderer *_renderer;

    std::vector<ISubsystems *> _subsystems;

private:
    bool BuildLogger();
    bool BuildEventSystem();
    bool BuildPlatform();
    bool BuildRenderer();

public:
    SubsystemFactory() {};
    ~SubsystemFactory() {};

public: // ISubsystem
    bool Startup() override;
    bool Shutdown() override;

public:
    ILogger *getLogger();
    IEvents *getEventSystem();
    IPlatform *getPlatform();
    IRenderer *getRenderer();
};
} // namespace ERI
