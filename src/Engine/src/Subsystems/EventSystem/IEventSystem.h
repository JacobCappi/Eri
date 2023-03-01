#pragma once
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
    virtual u64 SubscribeKeyPress(enum Keys, void (*callback)(i32, i32)) = 0;
    virtual u64 SubscribeMouse(enum Mouse, void (*callback)(i32, i32)) = 0;
    virtual u64 SubscribeInternal(enum Internal, void (*callback)(void)) = 0;

    virtual bool UnsubscribeKeyPress(enum Keys, u64 id) = 0;
    virtual bool UnsubscribeMouse(enum Mouse, u64 id) = 0;
    virtual bool UnsubscribeInternal(enum Internal, u64 id) = 0;

    virtual bool PublishKeyPress(enum Keys, i32 x, i32 y) = 0;
    virtual bool PublishMouse(enum Mouse, i32 x, i32 y) = 0;
    virtual bool PublishInternal(enum Internal) = 0;
};
    
} // namespace ERI