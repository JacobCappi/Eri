#pragma once

#include "core/application.h"

struct game
{
    struct app_configs configs;

    // Game state that the game itself will manage
    void* game_state;

    // function pointers to ask game instance to do everything
    b8 (*init)(struct game *game_instance);
    b8 (*update)(struct game *game_instance, f32 delta_time);
    b8 (*render)(struct game *game_instance, f32 delta_time);

    // function pointer game based resizing
    void (*on_resize)(struct game *game_instance, u32 width, u32 height);

};