#pragma once
#include "Subsystems/Events/InternalEvents/EventsInternal.h"


namespace ERI
{

class IRequestPlatformPointersSubscriber
{
public:
   virtual void onPointerRequest(void) = 0;
};

} // namespace ERI
