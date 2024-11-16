#pragma once

#include "Eri/Utils/Events/KeyPress/EventKeyPress.h"


namespace ERI
{

class IKeyPressSubscriber
{
public:
  virtual void onKeyEvent(enum KeyPressType, enum Keys) = 0;
};

} // namespace ERI
