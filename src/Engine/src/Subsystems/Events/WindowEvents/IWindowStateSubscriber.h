#pragma once
#include "Subsystems/Events/WindowEvents/EventWindowState.h"


namespace ERI
{

class IWindowStateSubscriber
{
public:
   virtual void onWindowStateEvent(enum WindowState, i32 x, i32 y) = 0;
};

} // namespace ERI

