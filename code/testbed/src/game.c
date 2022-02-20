#include "game.h"

// Stubs
b8 game_init(struct game* game_instance)
{
    return TRUE;
}

b8 game_update(struct game* game_instance, f32 delta_time)
{
    return TRUE;
}

b8 game_render(struct game* game_instance, f32 delta_time)
{
    return TRUE;
}

void game_on_resize(struct game* game_instance, u32 width, u32 height)
{
    return;
}