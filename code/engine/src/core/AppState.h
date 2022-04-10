#pragma once
#include "defines.h"

namespace ERI
{
    class AppState
    {
    private:
        b8 is_running;
        b8 is_suspended;
        i16 current_width;
        i16 current_height;
        f64 current_time;

    public:
        b8 get_is_running(void);
        b8 get_is_suspended(void);
        i16 get_width(void);
        i16 get_height(void);
        f64 get_time(void);

        b8 set_is_running(b8 running);
        b8 set_is_suspended(b8 suspended);
        b8 set_width(i16 w);
        b8 set_height(i16 h);
        b8 set_time(f64 time);

    };
}