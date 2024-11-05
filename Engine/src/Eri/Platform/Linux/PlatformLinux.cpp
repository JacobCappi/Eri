#include "Eri/Platform/Linux/PlatformLinux.h"
#include <time.h>

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
  XEvent e; 
  // When this returns false, the output buffer will
  // be flushed according to this:
  // https://tronche.com/gui/x/xlib/event-handling/manipulating-event-queue/XCheckMaskEvent.html
  while (XCheckMaskEvent(_display, _event_mask, &e))
  {
    _log->LogDebug("Event found: %d", e.type);
  }

  return true;
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

  _event_mask = 
    KeyPressMask | KeyReleaseMask |
    ButtonPressMask | ButtonReleaseMask |
    PointerMotionMask | StructureNotifyMask;


  XSelectInput(_display, _window, _event_mask);

  XMapWindow(_display, _window);

  // Wait for the MapNotify event that tells us
  // the window has been bound to our display
  while (true)
  {
    XEvent e;
    XNextEvent(_display, &e);
    if (e.type == MapNotify)
    {
      _log->LogInfo("X11 Window successfully bound to display");
      break;
    }
  }

  XFlush(_display);
  return true;
}

void PlatformLinux::clock_start()
{
  struct timespec current_time;
  clock_gettime(CLOCK_MONOTONIC, &current_time);
  _current_clock = current_time.tv_sec + 
      current_time.tv_nsec * 0.000000001;
}

f64 PlatformLinux::clock_delta()
{
  struct timespec current_time;
  clock_gettime(CLOCK_MONOTONIC, &current_time);
  f64 _now = current_time.tv_sec + 
      current_time.tv_nsec * 0.000000001;

  return _current_clock - _now;
}

void PlatformLinux::sleep(u64 ms)
{
  struct timespec ts;
  ts.tv_sec = ms / 1000;
  ts.tv_nsec = (ms % 1000) * 1000 * 1000;
  nanosleep(&ts, 0);
  return;

}

} // namespace ERI

