#pragma once
#include "Subsystems/Events/MouseEvents/EventMouse.h"


namespace ERI
{

class IMouseEventSubscriber
{
public:
   virtual void onMouseEvent(enum Mouse, i32 x, i32 y) = 0;
};
   
} // namespace ERI
