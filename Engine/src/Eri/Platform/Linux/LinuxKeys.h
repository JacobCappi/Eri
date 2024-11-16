#pragma once
#include "Eri/Core.h"

#include "Eri/Utils/Events/KeyPress/EventKeyPress.h"

namespace ERI
{

class LinuxKeys
{
public:
  static enum Keys translateKey(u32 x11_key);
};
} // namespace ERI