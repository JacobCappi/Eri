#include "Subsystems/EventSystem/VectorEvents/VectorEvents.h"


// TODO: this *could* waste lots of space if sub/unsub is in a loop
// Don't do this for now

namespace ERI
{
bool VectorEvents::Startup()
{
    if (_log == nullptr)
    {
        return false;
    }

    _log->LogInfo("Events System type Vector is starting up");
    // TODO: preallocate for optimization
    return true;
}

bool VectorEvents::Shutdown()
{
    _log->LogInfo("Events System type Vector is shutting down");
    _log = nullptr;

    return true;
}

// left param 0 is keydown, 1 is keyup
u64 VectorEvents::SubscribeKeyPress(void (*callback)(enum Keys, enum KeyPress))
{
    auto index = _keyPressCallbacks.begin() + _keyPressCount;
    _keyPressCallbacks.insert(index, callback);

    _log->LogDebug("Subscriber %d added to KeyPress", _keyPressCount);
    return _keyPressCount++;
}

u64 VectorEvents::SubscribeMouse(void (*callback)(enum Mouse, i32, i32))
{
    auto index = _mouseCallbacks.begin() + _mouseCount;
    _mouseCallbacks.insert(index, callback);

    _log->LogDebug("Subscriber %d added to Mouse", _mouseCount);
    return _mouseCount++;
}

u64 VectorEvents::SubscribeInternal(void (*callback)(enum Internal, i32, i32))
{
    auto index = _internalCallbacks.begin() + _internalCount;
    _internalCallbacks.insert(index, callback);

    _log->LogDebug("Subscriber %d added to Internal", _internalCount);
    return _internalCount++;
}

bool VectorEvents::UnsubscribeKeyPress(u64 id)
{
    auto index = _internalCallbacks.begin() + id;
    if (index >= _internalCallbacks.end())
    {
        _log->LogDebug("Subscriber %d not found in Key Press", id);
        return false;
    }


    _log->LogDebug("Subscriber %d removed from Key Press", id);
    // this *is* costly I think... we'll make a map change sometime
    _internalCallbacks.erase(index);
    return true;
}

bool VectorEvents::UnsubscribeMouse(u64 id)
{
    auto index = _mouseCallbacks.begin() + id;
    if (index >= _mouseCallbacks.end())
    {
        _log->LogDebug("Subscriber %d not found in Mouse", id);
        return false;
    }


    _log->LogDebug("Subscriber %d removed from Mouse", id);
    // this *is* costly I think... we'll make a map change sometime
    _mouseCallbacks.erase(index);
    return true;
}

bool VectorEvents::UnsubscribeInternal(u64 id)
{
    auto index = _internalCallbacks.begin() + id;
    if (index >= _internalCallbacks.end())
    {
        _log->LogDebug("Subscriber %d not found in Internal", id);
        return false;
    }


    _log->LogDebug("Subscriber %d removed from Internal", id);
    // this *is* costly I think... we'll make a map change sometime
    _internalCallbacks.erase(index);
    return true;
}

bool VectorEvents::PublishKeyPress(enum Keys key, enum KeyPress keyPress)
{
    if (_keyPressCallbacks.empty())
    {
        _log->LogInfo("No subscribers to this event");
        return false;
    }

    for (auto callbacks : _keyPressCallbacks)
    {
        callbacks(key, keyPress);
    }

    return true;
}

bool VectorEvents::PublishMouse(enum Mouse mouse, i32 x, i32 y)
{
    if (_mouseCallbacks.empty())
    {
        _log->LogInfo("No subscribers to this event");
        return false;
    }

    for (auto callbacks : _mouseCallbacks)
    {
        callbacks(mouse, x, y);
    }

    return true;
}

bool VectorEvents::PublishInternal(enum Internal internal, i32 left, i32 right)
{
    if (_internalCallbacks.empty())
    {
        _log->LogInfo("No subscribers to this event");
        return false;
    }

    for (auto callbacks : _internalCallbacks)
    {
        callbacks(internal, left, right);
    }

    return true;
}

void VectorEvents::RegisterLogger(ILogger *log)
{
    _log = log;
}

    
} // namespace ERI