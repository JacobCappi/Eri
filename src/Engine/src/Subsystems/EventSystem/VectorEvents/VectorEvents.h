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
    // TODO:
    // These parameters are based on window x,y.... in glut, they go from 0,0 -> windowsize, is this the case in Vulkan
    std::vector<void (*)(i32, i32)> KeyPressCallbacks[static_cast<u32>(Keys::MAX)];
    std::vector<void (*)(i32, i32)> MouseCallbacks[static_cast<u32>(Mouse::MAX)];
    std::vector<void (*)()> InternalCallbacks[static_cast<u32>(Internal::MAX)];

public:
    VectorEvents() {};
    ~VectorEvents() {};

public: // ISubsystem
    bool Startup() override;
    bool Shutdown() override;

public: // IEventSystem
    u64 SubscribeKeyPress(enum Keys, void (*callback)(i32, i32)) override;
    u64 SubscribeMouse(enum Mouse, void (*callback)(i32, i32)) override;
    u64 SubscribeInternal(enum Internal, void (*callback)(void)) override;

    bool UnsubscribeKeyPress(enum Keys, u64 id) override;
    bool UnsubscribeMouse(enum Mouse, u64 id) override;
    bool UnsubscribeInternal(enum Internal, u64 id) override;

    bool PublishKeyPress(enum Keys, i32 x, i32 y) override;
    bool PublishMouse(enum Mouse, i32 x, i32 y) override;
    bool PublishInternal(enum Internal) override;

    void RegisterLogger(ILogger *log) override;
};
} // namespace ERI
