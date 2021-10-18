#include "application.h"

#include "logger.h"
#include "platform/platform.h"

typedef struct app_state 
{
    b8 is_running;
    b8 is_suspended;
    i16 current_width;
    i16 current_height;
    f64 time;

    platform_state state;
} app_state;

static app_state singleton_app_state;
static b8 is_singleton_initialized = FALSE;

// All inits will be done here (excluding game specific code)
b8 app_create(app_configs* configs)
{
    if ( is_singleton_initialized )
    {
        ERI_LOG_ERROR("Application attempted to be created more than once");
        return FALSE;
    }

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
        configs->app_name,
        configs->start_x,
        configs->start_y,
        configs->start_width,
        configs->start_height
        ) 
    )
    {
        ERI_LOG_ERROR("Platform failed to start.");
        return FALSE;
    }

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
    }

    // incase anything weird happened
    singleton_app_state.is_running = FALSE;
    platform_shutdown(&singleton_app_state.state);

    return TRUE;
}