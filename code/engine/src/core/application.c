#include "core/application.h"

#include "core/clock.h"
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
    struct clock internal_clock;

    b8 is_running;
    b8 is_suspended;
    i16 current_width;
    i16 current_height;
    f64 time;
};

static struct app_state singleton_app_state;

b8 callback_on_event(u16 event_code, void *publisher, void *subsciber_instance, struct event_args data);
b8 callback_on_keypress(u16 event_code, void *publisher, void *subsciber_instance, struct event_args data);

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

    init_logging();
    init_memory();
    if ( !init_event() )
    {
        return FALSE;
    }
    init_input();

    event_subscribe(EVENT_APPLICATION_QUIT, 0, callback_on_event);
    event_subscribe(EVENT_KEY_PRESSED, 0, callback_on_keypress);
    event_subscribe(EVENT_KEY_RELEASED, 0, callback_on_keypress);

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
    clock_start(&singleton_app_state.internal_clock);
    clock_update(&singleton_app_state.internal_clock);
    singleton_app_state.time = singleton_app_state.internal_clock.elapsed;

    // TODO: refactor these names
    f64 runtime = 0;
    u8 frame_count = 0;
    f64 target_frame_out = 1.0f / 60;

    // TODO: memory leak, but just testing : REMOVE
    ERI_LOG_INFO(mem_get_status());

    while ( singleton_app_state.is_running )
    {
        if ( !platform_message(&singleton_app_state.state))
        {
            singleton_app_state.is_running = FALSE;
        }
        
        if ( !singleton_app_state.is_suspended )
        {
            // Update clock and get delta time.
            clock_update(&singleton_app_state.internal_clock);
            f64 current_time = singleton_app_state.internal_clock.elapsed;
            f64 delta = (current_time - singleton_app_state.time);
            f64 frame_start_time = platform_time();

            if ( !singleton_app_state.game_instance->update(singleton_app_state.game_instance, delta))
            {
                ERI_LOG_FATAL("Game failed to update state");
                singleton_app_state.is_running = FALSE;
                break;
            }
            if ( !singleton_app_state.game_instance->render(singleton_app_state.game_instance, delta))
            {
                ERI_LOG_FATAL("Game failed to render");
                singleton_app_state.is_running = FALSE;
                break;
            }


            // Figure out how long the frame took and, if below
            f64 frame_end_time = platform_time();
            f64 frame_elapsed_time = frame_end_time - frame_start_time;
            runtime += frame_elapsed_time;
            f64 remaining_seconds = target_frame_out - frame_elapsed_time;

            if (remaining_seconds > 0) {
                u64 remaining_ms = (remaining_seconds * 1000);

                // If there is time left, give it back to the OS.
                b8 limit_frames = FALSE;
                if (remaining_ms > 0 && limit_frames) {
                    platform_sleep(remaining_ms - 1);
                }

                frame_count++;
            }

            singleton_app_state.time = current_time;
            // Should be the last thing done every frame
            input_update(delta);
        }
    }

    if (singleton_app_state.is_running)
    {
        ERI_LOG_WARNING("System did not shutdown gracefully");
        singleton_app_state.is_running = FALSE;
    }

    event_unsubscribe(EVENT_APPLICATION_QUIT, 0, callback_on_event);
    event_unsubscribe(EVENT_KEY_PRESSED, 0, callback_on_keypress);
    event_unsubscribe(EVENT_KEY_RELEASED, 0, callback_on_keypress);

    shutdown_event();
    shutdown_input();

    shutdown_platform(&singleton_app_state.state);
    shutdown_memory();
    shutdown_logging();

    return TRUE;
}

b8 callback_on_event(u16 event_code, void *publisher, void *subsciber_instance, struct event_args data)
{
    if (event_code == EVENT_APPLICATION_QUIT)
    {
        ERI_LOG_INFO("Received Shutdown Event: Shutting Down System");
        singleton_app_state.is_running = FALSE;
        return TRUE;
    }
    return FALSE;
}

b8 callback_on_keypress(u16 event_code, void *publisher, void *subsciber_instance, struct event_args data)
{
    if (event_code == EVENT_KEY_PRESSED)
    {
        u16 key = data.u16[0];
        if (key == KEY_ESCAPE)
        {
            // TODO: Remove; This will close application on escape for debug reasons
            struct event_args data = {0};
            event_raise(EVENT_APPLICATION_QUIT, 0, data);
            return TRUE;
        }
        else
        {
            return TRUE;
        }

        // if it isn't the quit event, just let some other subsystem handle it
        return FALSE;
    }
    else if (event_code == EVENT_KEY_RELEASED)
    {
        u16 key = data.u16[0];
        return FALSE;

    }
    return FALSE;
}