#pragma once

#include "core/application.h"
#include "core/logger.h"
#include "core/mem.h"
#include "game_types.h"

extern b8 create_game(game *current_game);


// Entry Point to ERI
int main(void)
{
    // Init memory done here because subsystem startups
    init_memory();
    game game_instance;

    if ( !create_game(&game_instance) )
    {
        ERI_LOG_FATAL("Eri could not create this game");
        return -1;
    }

    if ( !game_instance.init || !game_instance.update || !game_instance.render || !game_instance.on_resize )
    {
        ERI_LOG_FATAL("One or more missing game function pointers. Make sure all function pointers exist");
        return -2;
    }

    if ( !app_create(&game_instance) )
    {
        ERI_LOG_INFO("Eri failed to create game instance");
        return 1;
    }

    // The Game Loop is app_run
    if( !app_run() )
    {
        ERI_LOG_INFO("Eri failed to shutdown gracefully ");
        return 2;
    }

    shutdown_memory();
    return 0;
}