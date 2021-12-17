#pragma once

#include <defines.h>
#include <game_types.h>

// Stubs for game code

struct game_state
{
    f32 delta_time;
};

b8 game_init(struct game* game_instance);
b8 game_update(struct game* game_instance, f32 delta_time);
b8 game_render(struct game* game_instance, f32 delta_time);
void game_on_resize(struct game* game_instance, u32 width, u32 height);