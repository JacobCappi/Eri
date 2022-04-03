#include "IGame.h"
#include "core/Application.h"
#include "core/Logger/Logger.h"
#include "core/Subsystems/Platform.h"


namespace ERI
{
    Application* Application::get_instance()
    {
        if (!singleton_app)
        {
            singleton_app = static_cast<Application *>(std::malloc(sizeof(Application)));
        }
        return singleton_app;
    }

    Application::~Application()
    {
        if (singleton_app)
        {
            free(singleton_app);
        }
    }

    b8 Application::start(IGame *game_instance)
    {
        platform.init(&log);
        game_instance->init();

        platform.init_windowing(
            game_instance->get_init_name().c_str(),
            game_instance->get_init_x(),
            game_instance->get_init_y(),
            game_instance->get_init_w(),
            game_instance->get_init_h()
        );

        app_configs.set_height(game_instance->get_init_h());
        app_configs.set_width(game_instance->get_init_w());

        game_instance->on_resize(
            app_configs.get_height(),
            app_configs.get_width()
        );

        app_configs.set_is_running(TRUE);
        app_configs.set_is_suspended(FALSE);

        log.log_debug() << "Testing Logging";

        return TRUE;
    }

    b8 Application::run(void)
    {
        while (app_configs.get_is_running())
        {
            if (!platform.pump_message())
            {
                app_configs.set_is_running(FALSE);
            }

            if (!app_configs.get_is_suspended())
            {
                if (game_instance->update((f32)0))
                {
                    log.log_warn() << "Game instance failed to update state.";
                    app_configs.set_is_running(FALSE);
                    break;
                }

                if (game_instance->render((f32)0))
                {
                    log.log_warn() << "Game instance failed to render";
                    app_configs.set_is_running(FALSE);
                    break;
                }
            }

        }

        platform.shutdown();
        app_configs.set_is_running(FALSE); ///< This should already be false, but just in case
        return TRUE;
    }

}