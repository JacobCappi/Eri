#pragma once
#include "Core.h"
#include "Subsystems/ISubsystems.h"
#include "Subsystems/Logger/ILogger.h"

#include "Subsystems/EventSystem/Events/EventKeyPress.h"
#include "Subsystems/EventSystem/Events/EventMouse.h"
#include "Subsystems/EventSystem/Events/EventInternal.h"

namespace ERI
{

// TODO: maybe interfaces (IPublisher, ISubscriber, etc..?)
class IEventSystem : public ISubsystems
{

public:
    IEventSystem() {};
    virtual ~IEventSystem() {};

public:
    virtual void RegisterLogger(ILogger *log) = 0;

public:
    virtual u64 SubscribeKeyPress(void (*callback)(enum Keys, enum KeyPress)) = 0;
    virtual u64 SubscribeMouse(void (*callback)(enum Mouse, i32, i32)) = 0;
    virtual u64 SubscribeInternal(void (*callback)(enum Internal, i32, i32)) = 0;

    virtual bool UnsubscribeKeyPress(u64 id) = 0;
    virtual bool UnsubscribeMouse(u64 id) = 0;
    virtual bool UnsubscribeInternal(u64 id) = 0;

    virtual bool PublishKeyPress(enum Keys, enum KeyPress) = 0;
    virtual bool PublishMouse(enum Mouse, i32 x, i32 y) = 0;
    virtual bool PublishInternal(enum Internal, i32 l, i32 r) = 0;
};
    
} // namespace ERI