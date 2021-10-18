#include "game.h"
#include <logger.h>

// Stubs
b8 game_init(game* game_instance)
{
    return TRUE;
}

b8 game_update(game* game_instance, f32 delta_time)
{
    return TRUE;
}

b8 game_render(game* game_instance, f32 delta_time)
{
    return TRUE;
}

void game_on_resize(game* game_instance, u32 width, u32 height)
{
    return;
}