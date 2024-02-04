#pragma once
#include "Subsystems/Events/KeyEvents/EventKeyPress.h"


namespace ERI
{

class IKeyEventSubscriber
{
public:
   virtual void onKeyEvent(enum KeyPress, enum Keys) = 0;
};

} // namespace ERI
