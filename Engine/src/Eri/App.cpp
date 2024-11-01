#include "App.h"

#include "Eri/Utils/EriUtils.h"
#include "Eri/Utils/IEvents.h"

#include "Eri/Platform/EriPlatform.h"

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

App::App(i32 x, i32 y, i32 width, i32 height, const char *app_name)
{
  _x_pos = x;
  _y_pos = y;
  _width = width;
  _height = height;
  _app_name = app_name;
}

App::~App()
{
}

void App::MainLoop()
{
  _isRunning = true;
  EriUtils utils = EriUtils();
  utils.Startup();

  auto platform = EriPlatform::getEriPlatform();
  _log = utils.getLogger();

  auto events = utils.getEventSystem();
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
