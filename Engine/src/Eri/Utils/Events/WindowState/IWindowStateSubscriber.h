#pragma once
#include "Eri/Core.h"

#include "Eri/Utils/Events/WindowState/EventWindowState.h"


namespace ERI
{

class IWindowStateSubscriber
{
public:
  virtual void onWindowStateEvent(enum WindowState, i32 x, i32 y) = 0;
};

} // namespace ERI

