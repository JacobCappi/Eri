#include "game.h"

#include <entry.h>
#include <core/mem.h>


// Definition of extern function defined in entry.h
b8 game_create(struct game* current_game)
{
    current_game->configs.start_x = 100;
    current_game->configs.start_y = 100;
    current_game->configs.start_height = 720;
    current_game->configs.start_width = 1280;
    current_game->configs.app_name = "Eri Engine";

    current_game->init  = game_init;
    current_game->update  = game_update;
    current_game->render  = game_render;
    current_game->on_resize = game_on_resize;

    current_game->game_state = eri_malloc( sizeof(struct game_state), MEM_GAME );

    return TRUE;
}