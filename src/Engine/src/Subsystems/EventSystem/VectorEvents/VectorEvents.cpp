#include "Subsystems/EventSystem/VectorEvents/VectorEvents.h"

namespace ERI
{
bool VectorEvents::Startup()
{
    if (_log == nullptr)
    {
        return false;
    }

    std::vector<void (*)(i32, i32)> test;
    _log->LogDebug("Size of Mouse Array %d", sizeof(MouseCallbacks)/sizeof(test));
    _log->LogDebug("Size of Key Array %d", sizeof(KeyPressCallbacks)/sizeof(test));
    _log->LogDebug("Size of Internal Array %d", sizeof(InternalCallbacks)/sizeof(test));

    _log->LogInfo("Events System type Vector is starting up");

    for (int i = 0; i < static_cast<u32>(Keys::MAX); ++i)
    {
        KeyPressCallbacks[i].reserve(4);
    }

    for (int i = 0; i < static_cast<u32>(Mouse::MAX); ++i)
    {
        MouseCallbacks[i].reserve(4);
    }

    for (int i = 0; i < static_cast<u32>(Internal::MAX); ++i)
    {
        InternalCallbacks[i].reserve(4);
    }

    return true;
}

bool VectorEvents::Shutdown()
{
    _log->LogInfo("Events System type Vector is shutting down");
    _log = nullptr;

    for (int i = 0; i < static_cast<u32>(Keys::MAX); ++i)
    {
        KeyPressCallbacks[i].clear();
    }

    for (int i = 0; i < static_cast<u32>(Mouse::MAX); ++i)
    {
        MouseCallbacks[i].clear();
    }

    for (int i = 0; i < static_cast<u32>(Internal::MAX); ++i)
    {
        InternalCallbacks[i].clear();
    }

    return true;
}

u64 VectorEvents::SubscribeKeyPress(enum Keys key, void (*callback)(i32, i32))
{
    u32 index = static_cast<u32>(key);
    if (index >= static_cast<u32>(Keys::MAX))
    {
        _log->LogError("Failed to subscribe to Key event, Event out of bounds.");
        return 0;
    }

    KeyPressCallbacks[index].push_back(callback);
    return KeyPressCallbacks[index].size();
}

u64 VectorEvents::SubscribeMouse(enum Mouse mouse, void (*callback)(i32, i32))
{
    u32 index = static_cast<u32>(mouse);
    if (index >= static_cast<u32>(Mouse::MAX))
    {
        _log->LogError("Failed to subscribe to Mouse event, Event out of bounds.");
        return 0;
    }

    MouseCallbacks[index].push_back(callback);
    return MouseCallbacks[index].size();
}

u64 VectorEvents::SubscribeInternal(enum Internal internal, void (*callback)(void))
{
    u32 index = static_cast<u32>(internal);
    if (index >= static_cast<u32>(Internal::MAX))
    {
        _log->LogError("Failed to subscribe to Internal event, Event out of bounds.");
    }

    InternalCallbacks[index].push_back(callback);
    return InternalCallbacks[index].size();
}

bool VectorEvents::UnsubscribeKeyPress(enum Keys key, u64 id)
{
    u32 index = static_cast<u32>(key);

    if (id > KeyPressCallbacks[index].size())
    {
        _log->LogWarning("Keys : Invalid Key, failed to unsubscribe");
        return false;
    }

    if (KeyPressCallbacks[index][id-1] == nullptr)
    {
        _log->LogWarning("Keys : Callback does not exist!");
        return false;
    }

    KeyPressCallbacks[index][id-1] = nullptr;
    return true;
}

bool VectorEvents::UnsubscribeMouse(enum Mouse mouse, u64 id)
{
    u32 index = static_cast<u32>(mouse);
    if (id > MouseCallbacks[index].size())
    {
        _log->LogWarning("Mouse: Invalid Key, failed to unsubscribe");
        return false;
    }

    if (MouseCallbacks[index][id-1] == nullptr)
    {
        _log->LogWarning("Mouse : Callback does not exist!");
        return false;
    }

    MouseCallbacks[index][id-1] = nullptr;
    return true;
}

bool VectorEvents::UnsubscribeInternal(enum Internal internal, u64 id)
{
    u32 index = static_cast<u32>(internal);
    if (id > InternalCallbacks[index].size())
    {
        _log->LogWarning("Mouse: Invalid Key, failed to unsubscribe");
        return false;
    }

    if (InternalCallbacks[index][id-1] == nullptr)
    {
        _log->LogWarning("Mouse : Callback does not exist!");
        return false;
    }

    InternalCallbacks[index][id-1] = nullptr;
    return true;

}

bool VectorEvents::PublishKeyPress(enum Keys key, i32 x, i32 y)
{
    u32 index = static_cast<u32>(key);
    if (KeyPressCallbacks[index].size() == 0)
    {
        _log->LogInfo("No subscribers to this event");
        return false;
    }

    for (i32 i = 0; i < KeyPressCallbacks[index].size(); ++i)
    {
        if (KeyPressCallbacks[index][i] == nullptr)
        {
            continue;
        }

        KeyPressCallbacks[index][i](x, y);
    }

    return true;
}

bool VectorEvents::PublishMouse(enum Mouse mouse, i32 x, i32 y)
{
    u32 index = static_cast<u32>(mouse);
    if (MouseCallbacks[index].size() == 0)
    {
        _log->LogInfo("No subscribers to this event");
        return false;
    }

    for (i32 i = 0; i < MouseCallbacks[index].size(); ++i)
    {
        if (MouseCallbacks[index][i] == nullptr)
        {
            continue;
        }

        MouseCallbacks[index][i](x, y);
    }

    return true;
}

bool VectorEvents::PublishInternal(enum Internal internal)
{
    u32 index = static_cast<u32>(internal);
    if (InternalCallbacks[index].size() == 0)
    {
        _log->LogInfo("No subscribers to this event");
        return false;
    }

    for (i32 i = 0; i < InternalCallbacks[index].size(); ++i)
    {
        if (InternalCallbacks[index][i] == nullptr)
        {
            continue;
        }

        InternalCallbacks[index][i]();
    }

    return true;
}

void VectorEvents::RegisterLogger(ILogger *log)
{
    _log = log;
}

    
} // namespace ERI