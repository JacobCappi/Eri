#pragma once
#include "Core.h"
#include "Subsystems/Events/KeyEvents/EventKeyPress.h"
#include "Subsystems/Logger/ILogger.h"

namespace ERI
{

/**
 * @brief Static class that converts windows virtual key codes into ERI key codes
 *
 * Windows has their own set of keycodes that define key presses. To make ERI
 * platform independent, a new set of common Keycodes were created for all
 * OS specific keycodes to be converted to
 *
 * @see https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
 * @param winKey the Windows keycode
 * @return enum a windows specific Keys
 */
class WindowKeys
{
public:
    static enum Keys translateKey(u16 winKey);
};
} // namespace ERI

