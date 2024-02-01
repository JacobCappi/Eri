#pragma once
#include "Subsystems/Events/InternalEvents/EventsInternal.h"


namespace ERI
{

class IReceivePlatformPointersSubscriber
{
public:
   // instance pointer, module pointer
   virtual void onPointerReceive(void *, void *) = 0;
};

} // namespace ERI
