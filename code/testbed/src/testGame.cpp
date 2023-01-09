#include "testGame.h"
#include <cstring>


testGame::~testGame()
{
    if (game_name)
    {
        delete game_name;
    }
}

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

ERI::String testGame::get_init_name()
{
    return *game_name;
}

b8 testGame::init(void)
{
    game_name = new ERI::String("Test Game");
// TODO: These are placeholder stubs
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
