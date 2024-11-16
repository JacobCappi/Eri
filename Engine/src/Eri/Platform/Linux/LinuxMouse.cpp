#include "Eri/Platform/Linux/LinuxMouse.h"

#include <X11/Xlib.h>

namespace ERI
{

Mouse LinuxMouse::translateMouse(i32 type, u32 x11_button)
{
  if (type == ButtonPress)
  {
    switch (x11_button)
    {
      case 1:
        return Mouse::LeftDown;
      case 2:
        return Mouse::MiddleDown;
      case 3:
        return Mouse::RightDown;
      case 4:
        return Mouse::ScrollUp;
      case 5:
        return Mouse::ScrollDown;
      case 8:
        return Mouse::ButtonFourDown;
      case 9:
        return Mouse::ButtonFiveDown;
      default:
        return Mouse::MAX;
    }
  }
  else if (type == ButtonRelease)
  {
    switch (x11_button)
    {
      case 1:
        return Mouse::LeftUp;
      case 2:
        return Mouse::MiddleUp;
      case 3:
        return Mouse::RightUp;
      case 4:
        return Mouse::ScrollUp;
      case 5:
        return Mouse::ScrollDown;
      case 8:
        return Mouse::ButtonFourUp;
      case 9:
        return Mouse::ButtonFiveUp;
      default:
        return Mouse::MAX;
    }
  }
  else
  {
    return Mouse::MAX;
  }
}
  
} // namespace ERI
