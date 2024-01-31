#pragma once

#include "Core.h"

#include "Subsystems/ISubsystems.h"
#include "Subsystems/Logger/ILogger.h"
#include "Subsystems/Events/IEvents.h"

#include "Subsystems/Events/MouseEvents/IMouseEventSubscriber.h"
#include "Subsystems/Events/KeyEvents/IKeyEventSubscriber.h"
#include "Subsystems/Events/WindowEvents/IWindowStateSubscriber.h"

#include <vector>

namespace ERI
{
class ERI_API VectorEvents : public IEvents
{

private:
    ILogger *_log;

private:
    u32 _keyPressCount = 0;
    u32 _mouseCount = 0;
    u32 _internalCount = 0;

private:
    std::vector<IKeyEventSubscriber *> _keyPressSubscribers;
    std::vector<IMouseEventSubscriber *> _mouseSubscribers;
    std::vector<IWindowStateSubscriber *> _windowStateSubscribers;

public:
    VectorEvents() {};
    ~VectorEvents() {};

public: // ISubsystem
    bool Startup() override;
    bool Shutdown() override;

public: // IEvents
    u64 SubscribeKeyPress(IKeyEventSubscriber *sub) override;
    u64 SubscribeMouse(IMouseEventSubscriber *sub) override;
    u64 SubscribeWindowState(IWindowStateSubscriber *sub) override;

    bool UnsubscribeKeyPress(u64 id) override;
    bool UnsubscribeMouse(u64 id) override;
    bool UnsubscribeWindowState(u64 id) override;

    bool PublishKeyPress(enum Keys, enum KeyPress) override;
    bool PublishMouse(enum Mouse, i32 x, i32 y) override;
    bool PublishWindowState(enum WindowState, i32 l, i32 r) override;

    void RegisterLogger(ILogger *log) override;
};
} // namespace ERI
