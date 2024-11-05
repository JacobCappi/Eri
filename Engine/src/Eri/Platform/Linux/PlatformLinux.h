#pragma once
#include "Eri/Core.h"

#include <X11/Xlib.h>

#include "Eri/Platform/IPlatform.h"

namespace ERI
{

class PlatformLinux : public IPlatform
{

private:
  i32 _windowX;
  i32 _windowY;
  i32 _windowWidth;
  i32 _windowHeight;

private:
  ILogger *_log;
  IEvents *_events;

private:
  Display *_display;
  Window _window;

public:
  PlatformLinux() {};
  ~PlatformLinux() {};


public: // IPlatform
  bool getPlatformMessage() override;
  void SetWindowSize(i32 width, i32 height) override;
  void SetWindowPosition(i32 x, i32 y) override;
  bool StartupWindow(const char *windowName) override;

  void registerUtils(EriUtils *const utils) override;

public:
  void clock_start() override;
  f64 clock_delta() override;
  void sleep(u64) override;

public:
  bool Startup() override;
  bool Shutdown() override;

};
  
} // namespace ERI