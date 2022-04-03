#pragma once
#include "defines.h"

#include "IGame.h"
#include "core/AppState.h"
#include "core/Subsystems/Platform.h"
#include "core/Logger/Logger.h"


namespace ERI
{
    class Application
    {

    private:
        Logger log;
        AppState app_configs;
        Platform platform;
        Application *singleton_app;
        IGame *game_instance;
        Application() = default;

    public:
        Application *get_instance(void);
        ~Application();

        b8 start(IGame *game_instance);
        b8 run(void);
    };

}