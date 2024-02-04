#pragma once
#include "Core.h"
#include "Subsystems/ISubsystems.h"
#include "Subsystems/Logger/ILogger.h"

#include "Subsystems/Events/KeyEvents/EventKeyPress.h"
#include "Subsystems/Events/KeyEvents/IKeyEventSubscriber.h"

#include "Subsystems/Events/MouseEvents/EventMouse.h"
#include "Subsystems/Events/MouseEvents/IMouseEventSubscriber.h"

#include "Subsystems/Events/WindowEvents/EventWindowState.h"
#include "Subsystems/Events/WindowEvents/IWindowStateSubscriber.h"


namespace ERI
{

// TODO: maybe interfaces (IPublisher, ISubscriber, etc..?)
class IEvents : public ISubsystems
{

public:
    IEvents() {};
    virtual ~IEvents() {};

public:
    virtual void RegisterLogger(ILogger *log) = 0;

public:
    virtual u64 SubscribeKeyPress(IKeyEventSubscriber *) = 0;
    virtual u64 SubscribeMouse(IMouseEventSubscriber *) = 0;
    virtual u64 SubscribeWindowState(IWindowStateSubscriber *) = 0;

    virtual bool UnsubscribeKeyPress(u64 id) = 0;
    virtual bool UnsubscribeMouse(u64 id) = 0;
    virtual bool UnsubscribeWindowState(u64 id) = 0;

    virtual bool PublishKeyPress(enum Keys, enum KeyPress) = 0;
    virtual bool PublishMouse(enum Mouse, i32 x, i32 y) = 0;
    virtual bool PublishWindowState(enum WindowState, i32 l, i32 r) = 0;

};

} // namespace ERI