#pragma once
#include "Eri/Core.h"

#include "Eri/Platform/IPlatform.h"

namespace ERI
{

enum class Platforms
{
  WINDOWS,
  LINUX_X11,
  LINUX_WAYLAND,
  UNSUPPORTED
};

class EriPlatform
{
private:
  IPlatform *_platform;

public:
  EriPlatform() {};
  ~EriPlatform() {};

public:
  IPlatform *getEriPlatform();
  Platforms platformType();

public:
  bool Startup();
  bool Shutdown();
};

}