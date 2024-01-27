#pragma once
#include "Core.h"
#include "Subsystems/EventSystem/Events/EventKeyPress.h"
#include "Subsystems/Logger/ILogger.h"

namespace ERI
{
class WindowKeys
{
public:
    WindowKeys() {}
    ~WindowKeys() {}

public:
    enum Keys translateKey(u16 winKey);

};
} // namespace ERI

