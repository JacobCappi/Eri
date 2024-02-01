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
    IRenderer *_renderer;

    std::vector<ISubsystems *> _subsystems;

private:
    i32 _x, _y, _width, _height;
    const char *_app_name;

private:
    bool BuildLogger();
    bool BuildEventSystem();
    bool BuildPlatform();
    bool BuildRenderer();

public:
    Factory(i32 x, i32 y, i32 w, i32 h, const char *name);
    ~Factory() {};

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
