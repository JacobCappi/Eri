#include "Subsystems/Events/VectorEvents/VectorEvents.h"


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

u64 VectorEvents::SubscribeKeyPress(IKeyEventSubscriber *sub)
{
    auto index = _keyPressSubscribers.begin() + _keyPressCount;
    _keyPressSubscribers.insert(index, sub);

    _log->LogDebug("Subscriber %d added to KeyPress", _keyPressCount);
    return _keyPressCount++;
}

u64 VectorEvents::SubscribeMouse(IMouseEventSubscriber *sub)
{
    auto index = _mouseSubscribers.begin() + _mouseCount;
    _mouseSubscribers.insert(index, sub);

    _log->LogDebug("Subscriber %d added to Mouse", _mouseCount);
    return _mouseCount++;
}

u64 VectorEvents::SubscribeWindowState(IWindowStateSubscriber *sub)
{
    auto index = _windowStateSubscribers.begin() + _internalCount;
    _windowStateSubscribers.insert(index, sub);

    _log->LogDebug("Subscriber %d added to Internal", _internalCount);
    return _internalCount++;
}

bool VectorEvents::UnsubscribeKeyPress(u64 id)
{
    auto index = _windowStateSubscribers.begin() + id;
    if (index >= _windowStateSubscribers.end())
    {
        _log->LogDebug("Subscriber %d not found in Key Press", id);
        return false;
    }


    _log->LogDebug("Subscriber %d removed from Key Press", id);
    // this *is* costly I think... we'll make a map change sometime
    _windowStateSubscribers.erase(index);
    return true;
}

bool VectorEvents::UnsubscribeMouse(u64 id)
{
    auto index = _mouseSubscribers.begin() + id;
    if (index >= _mouseSubscribers.end())
    {
        _log->LogDebug("Subscriber %d not found in Mouse", id);
        return false;
    }


    _log->LogDebug("Subscriber %d removed from Mouse", id);
    // this *is* costly I think... we'll make a map change sometime
    _mouseSubscribers.erase(index);
    return true;
}

bool VectorEvents::UnsubscribeWindowState(u64 id)
{
    auto index = _windowStateSubscribers.begin() + id;
    if (index >= _windowStateSubscribers.end())
    {
        _log->LogDebug("Subscriber %d not found in Internal", id);
        return false;
    }


    _log->LogDebug("Subscriber %d removed from Internal", id);
    // this *is* costly I think... we'll make a map change sometime
    _windowStateSubscribers.erase(index);
    return true;
}

bool VectorEvents::PublishKeyPress(enum Keys key, enum KeyPress keyPress)
{
    if (_keyPressSubscribers.empty())
    {
        return false;
    }

    for (auto subscribers : _keyPressSubscribers)
    {
        subscribers->onKeyEvent(keyPress, key);
    }

    return true;
}

bool VectorEvents::PublishMouse(enum Mouse mouse, i32 x, i32 y)
{
    if (_mouseSubscribers.empty())
    {
        return false;
    }

    for (auto subscribers : _mouseSubscribers)
    {
        subscribers->onMouseEvent(mouse, x, y);
    }

    return true;
}

bool VectorEvents::PublishWindowState(enum WindowState state, i32 left, i32 right)
{
    if (_windowStateSubscribers.empty())
    {
        return false;
    }

    for (auto subscribers : _windowStateSubscribers)
    {
        subscribers->onWindowStateEvent(state, left, right);
    }

    return true;
}

void VectorEvents::RegisterLogger(ILogger *log)
{
    _log = log;
}

    
} // namespace ERI