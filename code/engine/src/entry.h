#include "core/application.h"
#include "core/logger.h"
#include "game_types.h"

extern b8 create_game(game *current_game);


// Entry Point to Application
int main(void)
{

    game game_instance;
    if ( !create_game(&game_instance) )
    {
        ERI_LOG_FATAL("Eri could not create this game");
        return -1;
    }

    if ( !(game_instance.initialize || game_instance.update || game_instance.render || game_instance.on_resize) )
    {
        ERI_LOG_FATAL("One or more missing game function pointers. Make sure all function pointers exist");
        return -2;
    }

    app_configs config;
    config.start_x = 100;
    config.start_y = 100;
    config.start_height = 720;
    config.start_width = 1280;
    config.app_name = "Eri Engine Testbed";

    app_create(&config);
    app_run();

    return 0;
}