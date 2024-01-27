#include "App.h"

#include "Subsystems/Factory.h"
#include "Subsystems/Subsystems.h"

namespace ERI
{

ILogger *_log;

void test(enum Keys key, enum KeyPress press)
{
    if (press == KeyPress::DOWN)
    {
        _log->LogInfo("%d was passed in", key);
    }
}

void testMouse(enum Mouse button, i32 x, i32 y)
{
    _log->LogInfo("Mouse with %d at: (%d, %d)", button, x, y);
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

    // auto events = factory.getEventSystem();
    // events->SubscribeKeyPress(test);
    // events->SubscribeMouse(testMouse);

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

        _log->LogDebug("Time per frame %f real %f", _time_per_frame, platform->clock_delta());

    }
}
    
} // namespace ERI
