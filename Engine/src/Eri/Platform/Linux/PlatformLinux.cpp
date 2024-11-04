#include "Eri/Platform/Linux/PlatformLinux.h"

// https://www.tronche.com/gui/x/xlib-tutorial/2nd-program-anatomy.html

namespace ERI
{
void PlatformLinux::registerUtils(EriUtils *const utils)
{
  _log = utils->getLogger();
  _events = utils->getEventSystem();
}

bool PlatformLinux::Startup()
{
  if (_log == nullptr || _events == nullptr)
  {
    return false;
  }

  _windowX = 0;
  _windowY = 0;
  _windowWidth = 0;
  _windowHeight = 0;

  _log->LogInfo("Created Platform System type Linux");
  return true;
}

bool PlatformLinux::Shutdown()
{
  _log = nullptr;
  _events = nullptr;

  XDestroyWindow(_display, _window);
  return XCloseDisplay(_display);
}


bool PlatformLinux::getPlatformMessage()
{
  return false;

}

void PlatformLinux::SetWindowSize(i32 width, i32 height)
{
  _windowWidth = width;
  _windowHeight = height;
}

void PlatformLinux::SetWindowPosition(i32 x, i32 y)
{
  _windowX = x;
  _windowY = y;
}

bool PlatformLinux::StartupWindow(const char *windowName)
{
  if (_windowHeight == 0 || _windowWidth == 0)
  {
    _log->LogError("Window sizes not set before window creation");
    return false;
  }

  _display = XOpenDisplay(NULL);

  if (!_display)
  {
    _log->LogError("Failed to connect to XServer");
  }

  // According to the doc, these are the only 2 colors are guaranteed
  // so we use them for our borders
  int color_black = BlackPixel(_display, DefaultScreen(_display));
  int color_white = WhitePixel(_display, DefaultScreen(_display));

  /**
   * 
  Window XCreateSimpleWindow(Display *display, Window parent, int  x,  int  y,
        unsigned  int  width, unsigned int height, unsigned int border_width,
        unsigned long border, unsigned long background);
   */
  _window = XCreateSimpleWindow(
    _display,
    DefaultRootWindow(_display),
    _windowX, _windowY,
    _windowWidth, _windowHeight,
    2, color_white, color_black
  );


  return true;
}

void PlatformLinux::clock_start()
{

return;
}

f64 PlatformLinux::clock_delta()
{
  return -1;


}

void PlatformLinux::sleep(u64)
{
  return;

}

} // namespace ERI

