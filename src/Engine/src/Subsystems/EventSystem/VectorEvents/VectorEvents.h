#pragma once

#include "Core.h"

#include "Subsystems/ISubsystems.h"
#include "Subsystems/Logger/ILogger.h"
#include "Subsystems/EventSystem/IEventSystem.h"

#include "Subsystems/EventSystem/Events/EventKeyPress.h"
#include "Subsystems/EventSystem/Events/EventMouse.h"
#include "Subsystems/EventSystem/Events/EventInternal.h"

#include <vector>

namespace ERI
{
class ERI_API VectorEvents : public IEventSystem
{

private:
    ILogger *_log;

private:
    u32 _keyPressCount = 0;
    u32 _mouseCount = 0;
    u32 _internalCount = 0;

private:
    std::vector<void (*)(enum Keys, enum KeyPress)> _keyPressCallbacks;
    std::vector<void (*)(enum Mouse, i32, i32)> _mouseCallbacks;
    std::vector<void (*)(enum Internal, i32, i32)> _internalCallbacks;

public:
    VectorEvents() {};
    ~VectorEvents() {};

public: // ISubsystem
    bool Startup() override;
    bool Shutdown() override;

public: // IEventSystem
    u64 SubscribeKeyPress(void (*callback)(enum Keys, enum KeyPress)) override;
    u64 SubscribeMouse(void (*callback)(enum Mouse, i32, i32)) override;
    u64 SubscribeInternal(void (*callback)(enum Internal, i32, i32)) override;

    bool UnsubscribeKeyPress(u64 id) override;
    bool UnsubscribeMouse(u64 id) override;
    bool UnsubscribeInternal(u64 id) override;

    bool PublishKeyPress(enum Keys, enum KeyPress) override;
    bool PublishMouse(enum Mouse, i32 x, i32 y) override;
    bool PublishInternal(enum Internal, i32 l, i32 r) override;

    void RegisterLogger(ILogger *log) override;
};
} // namespace ERI
