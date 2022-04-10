#pragma once
#include "defines.h"
#include "IGame.h"
#include "core/Logger/Logger.h"
#include "core/Application.h"

// TODO: More than likely a more C++ way to do this
extern ERI::IGame* create_game(void);

int main(void)
{
    ERI::Application *app;
    ERI::IGame *game;
    ERI::Logger log;

    app = app->get_instance();
    game = create_game(); ///< TODO: find a more C++ way to do this

    if (!app->start(game))
    {
        log.log_warn() << "Eri failed to start";
        return 1;
    }

    if (!app->run())
    {
        log.log_warn() << "Eri failed to shutdown gracefully";
        return 1;
    }

    return 0;
}