#include "core/application.h"

#include "core/event.h"
#include "core/input.h"
#include "core/logger.h"
#include "core/mem.h"
#include "platform/platform.h"
#include "game_types.h"

// Current state of Application
struct app_state 
{
    struct platform_state state;
    struct game* game_instance;

    b8 is_running;
    b8 is_suspended;
    i16 current_width;
    i16 current_height;
    f64 time;
};

static struct app_state singleton_app_state;

// Singleton being handled using this static boolean
static b8 is_singleton_init = FALSE;

b8 app_create(struct game* game_instance)
{
    ERI_LOG_INFO("Eri starting up...");
    if ( is_singleton_init )
    {
        ERI_LOG_ERROR("Application attempted to be created more than once");
        return FALSE;
    }

    singleton_app_state.game_instance = game_instance;

    if ( !init_platform(
            &singleton_app_state.state,
            game_instance->configs.app_name,
            game_instance->configs.start_x,
            game_instance->configs.start_y,
            game_instance->configs.start_width,
            game_instance->configs.start_height ) 
        )
    {
        ERI_LOG_ERROR("Platform failed to start.");
        return FALSE;
    }

    ERI_LOG_INFO("Initializing: [ Game ]");
    if ( !singleton_app_state.game_instance->init(game_instance) )
    {
        ERI_LOG_FATAL("Failed to initialize [ Game ]");
        return FALSE;
    }
// ----- END

    // TODO: Hook up event handler for this
    singleton_app_state.game_instance->on_resize(
        singleton_app_state.game_instance,
        singleton_app_state.current_width,
        singleton_app_state.current_height);


    singleton_app_state.is_running = TRUE;
    singleton_app_state.is_suspended = FALSE;
    is_singleton_init = TRUE;


    return TRUE;
}

// The Game loop
b8 app_run(void)
{
    // TODO: memory leak, but just testing : REMOVE
    ERI_LOG_INFO(get_mem_status());

    while ( singleton_app_state.is_running )
    {
        if ( !platform_message(&singleton_app_state.state))
        {
            singleton_app_state.is_running = FALSE;
        }
        
        if ( !singleton_app_state.is_suspended )
        {
            if ( !singleton_app_state.game_instance->update(singleton_app_state.game_instance, (f32)0))
            {
                ERI_LOG_FATAL("Game failed to update state");
                singleton_app_state.is_running = FALSE;
                break;
            }
            if ( !singleton_app_state.game_instance->render(singleton_app_state.game_instance, (f32)0))
            {
                ERI_LOG_FATAL("Game failed to render");
                singleton_app_state.is_running = FALSE;
                break;
            }

            update_input(0);
        }
    }

    // incase anything weird happened
    singleton_app_state.is_running = FALSE;

    ERI_LOG_INFO("Eri shutting down...");
    shutdown_event();
    shutdown_input();

    shutdown_platform(&singleton_app_state.state);
    shutdown_logging();
    shutdown_memory();

    return TRUE;
}