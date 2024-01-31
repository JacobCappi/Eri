#include "App.h"

#include "Subsystems/Factory.h"
#include "Subsystems/Subsystems.h"

#include <vulkan/vulkan.h>

namespace ERI
{

void App::onKeyEvent(enum KeyPress press, enum Keys key)
{
    if (press == KeyPress::DOWN)
        _log->LogDebug("Key press down %d", key);
}

void App::onMouseEvent(enum Mouse mouse, i32 x, i32 y)
{
    if (mouse != Mouse::Move)
        _log->LogDebug("Mouse %d at (%d, %d)", mouse, x, y);
}

void App::onWindowStateEvent(enum WindowState state, i32 x, i32 y)
{
    if (state == WindowState::WindowResize)
    {
        _log->LogDebug("New width %d, height %d", x, y);
    }
}

App::App()
{
}
App::~App()
{
}

void App::MainLoop()
{
    _isRunning = true;
    Factory factory = Factory();
    factory.Startup();

    auto platform = factory.getPlatform();
    _log = factory.getLogger();
    platform->SetWindowPosition(100, 100);
    platform->SetWindowSize(800, 800);
    platform->StartupWindow("Eri Engine");

    auto events = factory.getEventSystem();
    events->SubscribeKeyPress(this);
    events->SubscribeMouse(this);
    events->SubscribeWindowState(this);

    while (_isRunning)
    {
        platform->clock_start();

        if (!platform->getPlatformMessage())
        {
            _isRunning = false;
        }

        _frame_time = platform->clock_delta();
        if (_frame_time < _time_per_frame)
        {
            platform->sleep(_time_per_frame - _frame_time);
        }

        //_log->LogDebug("Time per frame %f real %f", _time_per_frame, platform->clock_delta());

    }
}
    
} // namespace ERI
