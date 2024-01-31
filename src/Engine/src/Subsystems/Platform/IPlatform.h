#pragma once

#include "Core.h"

#include "Subsystems/ISubsystems.h"
#include "Subsystems/Logger/ILogger.h"
#include "Subsystems/Events/IEvents.h"

// TODO: figure out the windowing part of this interface after vulkan or linux part
namespace ERI
{

class IPlatform : public ISubsystems
{

public:
    IPlatform() {};
    virtual ~IPlatform() {};

public:
    virtual void registerLogger(ILogger *log) = 0;
    virtual void registerEvents(IEvents *events) = 0;

public:
// Based it off glut(?)
    virtual bool getPlatformMessage() = 0;
    virtual void SetWindowSize(i32 width, i32 height) = 0;
    virtual void SetWindowPosition(i32 x, i32 y) = 0;
    virtual bool StartupWindow(const char *windowName) = 0;

public:
    // sets the current time
    virtual void clock_start() = 0;

    // returns time elapsed from start in ms
    virtual f64 clock_delta() = 0;

    virtual void sleep(u64) = 0;
};
    
} // namespace ERI
