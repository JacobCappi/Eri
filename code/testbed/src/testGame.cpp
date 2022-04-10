#include "testGame.h"
#include <cstring>

i16 testGame::get_init_x(void)
{
    return starting_x;
}

i16 testGame::get_init_y(void)
{
    return starting_y;
}

i16 testGame::get_init_w(void)
{
    return starting_w;
}

i16 testGame::get_init_h(void)
{
    return starting_h;
}

b8 testGame::get_init_name(char *buff, u16 size)
{
    b8 success = (std::strlen(game_name) < size);
    if (success)
    {
        std::strcpy(buff, game_name);
    }
    return success;
}

// TODO: These a placeholder stubs
b8 testGame::init(void)
{
    return init_size(100, 100, 1280, 720);
}

b8 testGame::init_size(i16 x, i16 y, i16 w, i16 h)
{
    starting_x = x;
    starting_y = y;
    starting_w = w;
    starting_h = h;
    return TRUE;
}

b8 testGame::update(f32 delta_time)
{
    return TRUE;
}

b8 testGame::render(f32 delta_time)
{
    return TRUE;
}

b8 testGame::on_resize(u32 w, u32 h)
{
    return TRUE;
}
