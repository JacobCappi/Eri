#pragma once
#include "Eri/Core.h"

#include "Eri/Utils/Events/Mouse/EventMouse.h"

namespace ERI
{

class IMouseSubscriber
{
public:
  virtual void onMouseEvent(enum Mouse, i32 x, i32 y) = 0;
};

} // namespace ERI
