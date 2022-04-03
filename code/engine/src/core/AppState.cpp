#include "core/AppState.h"

namespace ERI
{
    b8 AppState::get_is_running(void)
    {
        return is_running;
    }

    b8 AppState::get_is_suspended(void)
    {
        return is_suspended;
    }
    
    i16 AppState::get_width(void)
    {
        return current_width;
    }

    i16 AppState::get_height(void)
    {
        return current_height;
    }

    f64 AppState::get_time(void)
    {
        return current_time;
    }

    b8 AppState::set_is_running(b8 running)
    {
        b8 success = running == FALSE || running == TRUE;
        if (success)
        {
            is_running = running;
        }
        return success;
    }

    b8 AppState::set_is_suspended(b8 suspended)
    {
        b8 success = suspended == FALSE || suspended == TRUE;
        if (success)
        {
            is_suspended = suspended;
        }
        return success;
    }

    b8 AppState::set_width(i16 w)
    {
        // TODO: Get max width/height up and running
        b8 success = w > 0;
        if (success)
        {
            current_width = w;
        }
        return success;
    }

    b8 AppState::set_height(i16 h)
    {
        b8 success = h > 0;
        if (success)
        {
            current_height = h;
        }
        return success;
    }

    b8 AppState::set_time(f64 time)
    {
        b8 success = time > 0;
        if (success)
        {
            current_time = time;
        }
        return success;
    }


} 
