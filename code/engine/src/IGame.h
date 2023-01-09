#pragma once
#include "defines.h"
#include "core/DataStructs/Strings.h"


namespace ERI
{
    class IGame
    {
        // TODO: maybe once camera is understood, we can put an ICamera in here
    public:
        virtual i16 get_init_x(void) = 0;
        virtual i16 get_init_y(void) = 0;
        virtual i16 get_init_w(void) = 0;
        virtual i16 get_init_h(void) = 0;
        virtual String get_init_name() = 0;

        virtual b8 init(void) = 0;
        virtual b8 update(f32 delta_time) = 0;
        virtual b8 render(f32 delta_time) = 0;
        virtual b8 on_resize(u32 w, u32 h) = 0;
    };

}
