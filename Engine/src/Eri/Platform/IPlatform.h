#pragma once
#include "Eri/Core.h"

#include "Eri/Utils/EriUtils.h"

// TODO: figure out the windowing part of this interface after vulkan or linux part
namespace ERI
{

/**
 * @brief Interface that defines all properties of a supported ERI Platform
 */
class IPlatform
{

public:
  IPlatform() {};
  virtual ~IPlatform() {};

public:
  virtual void registerUtils(EriUtils *const utils) = 0;

public:
// Based it off glut(?)
  virtual bool getPlatformMessage() = 0;
  virtual void SetWindowSize(i32 width, i32 height) = 0;
  virtual void SetWindowPosition(i32 x, i32 y) = 0;
  virtual bool StartupWindow(const char *windowName) = 0;

public:
  virtual bool getWindowPointers(const void *window, const void **display) = 0;
  virtual bool getWindowExtention(const char **VKExtention) = 0;

public:
  // sets the current time
  virtual void clock_start() = 0;

  // returns time elapsed from start in ms
  virtual f64 clock_delta() = 0;

  virtual void sleep(u64) = 0;

public:
  virtual bool Startup() = 0;
  virtual bool Shutdown() = 0;
};

} // namespace ERI
