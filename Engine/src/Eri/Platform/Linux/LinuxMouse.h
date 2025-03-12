#pragma once
#include "Eri/Core.h"

#include "Eri/Utils/Events/Mouse/EventMouse.h"

namespace ERI
{
class LinuxMouse
{
public:
  static Mouse translateMouse(i32 type, u32 x11_button);
};

} // namespace ERI
