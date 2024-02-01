#pragma once
#include "Core.h"
#include "Subsystems/Events/KeyEvents/EventKeyPress.h"
#include "Subsystems/Logger/ILogger.h"

namespace ERI
{
class WindowKeys
{
public:
    static enum Keys translateKey(u16 winKey);
};
} // namespace ERI

