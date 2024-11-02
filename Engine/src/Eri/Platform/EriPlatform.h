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
public:
  static IPlatform *getEriPlatform();
  static Platforms platformType();
};

}