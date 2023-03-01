#pragma once

#include "Subsystems/ISubsystems.h"
#include "Subsystems/Logger/ILogger.h"
#include "Subsystems/EventSystem/IEventSystem.h"
#include "Core.h"

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
    virtual void registerEvents(IEventSystem *events) = 0;

public:
    virtual bool getPlatformMessage() = 0;
    virtual void SetWindowSize(i32 width, i32 height) = 0;
    virtual void SetWindowPosition(i32 x, i32 y) = 0;
    virtual bool StartupWindow(const char *windowName) = 0;

// Based it off glut(?)



};
    
} // namespace ERI
