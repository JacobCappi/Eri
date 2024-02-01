#pragma once
#include "Subsystems/Events/InternalEvents/EventsInternal.h"


namespace ERI
{

class IVulkanStringSubscriber
{
public:
   virtual void onStringRequest(void) = 0;
};

} // namespace ERI
