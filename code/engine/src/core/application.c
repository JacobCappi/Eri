#include "application.h"

#include "logger.h"
#include "game_types.h"
#include "platform/platform.h"

typedef struct app_state 
{
    b8 is_running;
    b8 is_suspended;
    i16 current_width;
    i16 current_height;
    f64 time;

    game* game_instance;
    platform_state state;
} app_state;

static app_state singleton_app_state;
static b8 is_singleton_initialized = FALSE;

// All inits will be done here (excluding game specific code)
b8 app_create(game* game_instance)
{
    if ( is_singleton_initialized )
    {
        ERI_LOG_ERROR("Application attempted to be created more than once");
        return FALSE;
    }

    singleton_app_state.game_instance = game_instance;
// ----- Init all Subsystems
    init_logging();
// ----- END

// TODO: Remove once done testing logger
    ERI_LOG_FATAL("Testing %s %s", "testing", "working");
    ERI_LOG_ERROR("Testing %s %s", "testing", "working");
    ERI_LOG_WARNING("Testing %s %s", "testing", "working");
    ERI_LOG_INFO("Testing %s %s", "testing", "working");
    ERI_LOG_DEBUG("Testing %s %s", "testing", "working");
    ERI_LOG_TRACE("Testing %s %s", "testing", "working");
// ----- END

    if ( 
        !platform_startup(
        &singleton_app_state.state,
        game_instance->configs.app_name,
        game_instance->configs.start_x,
        game_instance->configs.start_y,
        game_instance->configs.start_width,
        game_instance->configs.start_height
        ) 
    )
    {
        ERI_LOG_ERROR("Platform failed to start.");
        return FALSE;
    }

    if ( !singleton_app_state.game_instance->init(game_instance) )
    {
        ERI_LOG_FATAL("Game failed to init");
        return FALSE;
    }

    // TODO: Hook up event handler for this
    singleton_app_state.game_instance->on_resize(
        singleton_app_state.game_instance,
        singleton_app_state.current_width,
        singleton_app_state.current_height);


    singleton_app_state.is_running = TRUE;
    singleton_app_state.is_suspended = FALSE;
    is_singleton_initialized = TRUE;

    return TRUE;
}

b8 app_run(void)
{
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
                ERI_LOG_FATAL("Game Failed to update state");
                singleton_app_state.is_running = FALSE;
                break;
            }
            if ( !singleton_app_state.game_instance->render(singleton_app_state.game_instance, (f32)0))
            {
                ERI_LOG_FATAL("Game Failed to render");
                singleton_app_state.is_running = FALSE;
                break;
            }
        }
    }

    // incase anything weird happened
    singleton_app_state.is_running = FALSE;
    platform_shutdown(&singleton_app_state.state);

    return TRUE;
}