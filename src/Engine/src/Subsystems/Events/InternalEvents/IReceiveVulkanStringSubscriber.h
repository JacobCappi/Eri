#pragma once
#include "Subsystems/Events/InternalEvents/EventsInternal.h"


namespace ERI
{

class IReceiveVulkanStringSubscriber
{
public:
   // instance pointer, module pointer
   virtual void onStringReceive(const char *) = 0;
};

} // namespace ERI
