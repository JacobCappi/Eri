#include "core/Subsystems/Platform.h"

#if ERI_PLATFORM_LINUX

#include "core/Logger/Logger.h"

// (libs) libx11-dev, libxkbcommon-x11-dev 
// xcb is a newer lib
#include <xcb/xcb.h>
#include <X11/keysym.h>
#include <X11/XKBlib.h> 
#include <X11/Xlib.h>
#include <X11/Xlib-xcb.h> 
#include <sys/time.h>

// nanosleep vs usleep
#if _POSIX_C_SOURCE >= 199309L
    #include <time.h>
#else
    #include <unistd.h>
#endif

// TODO: move these out once we build these systems
#include <stdlib.h>
#include <string>
#include <cstring>


namespace ERI
{

    struct linux_windowing 
    {
        Display *display; // xLib portion
        xcb_connection_t *connection;
        xcb_window_t window;
        xcb_screen_t *screen;
        xcb_atom_t wm_protocols;
        xcb_atom_t wm_delete_window;
    };

    b8 Platform::init(Logger *logger)
    {
        log = logger;
        const std::string name = "Platform Linux";
        set_subsystem_name(name);

        return (log == nullptr) ? FALSE : TRUE;
    }

    void Platform::shutdown()
    {
        if (abstract_wnd_state)
        {
            struct linux_windowing *state = (struct linux_windowing *)abstract_wnd_state;
            // XAutoRepeatOn(state->display);
            xcb_destroy_window(state->connection, state->window);
        }
    }

    void Platform::set_subsystem_name(const std::string name)
    {
        subsystem_name = name;
    }

    const std::string Platform::get_name(void)
    {
        return subsystem_name;
    }

    b8 Platform::init_windowing(const char *wnd_name, i32 x, i32 y, i32 width, i32 height)
    {
        abstract_wnd_state = malloc(sizeof(struct linux_windowing), FALSE);
        if (!log)
        {
            // Logger should destruct here right?
            // Find better solution if constructor not called?
            // or can I assume constructor is called in cpp?
            Logger tmp_logger;
            tmp_logger.log_fatal() << "Failed to init window. Constructor not called";
            return FALSE;
        }
        struct linux_windowing *state = (struct linux_windowing *)abstract_wnd_state;

        // Inits for the abstract_wnd_state to send to rendering system
        state->display = XOpenDisplay(NULL);
        state->connection = XGetXCBConnection(state->display);

        // TODO: Stick the other side in destructor
        // XAutoRepeatOff(state->display);

        if (xcb_connection_has_error(state->connection))
        {
            log->log_fatal() << "XCB failed to connect to X11 server";
            return FALSE;
        }

        // Get the colors, size, etc... from X server
        const struct xcb_setup_t *xcb_setup = xcb_get_setup(state->connection);
        // Then gets the first screen to set our struct
        xcb_screen_iterator_t screen_iterator = xcb_setup_roots_iterator(xcb_setup);

        // Set screen and set our window id
        state->screen = screen_iterator.data;
        state->window = xcb_generate_id(state->connection);

        // Register event types.
        // XCB_CW_EVENT_MASK is required.
        u32 event_mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;

        // Listen for keyboard and mouse buttons
        u32 event_values = XCB_EVENT_MASK_BUTTON_PRESS | XCB_EVENT_MASK_BUTTON_RELEASE |
                        XCB_EVENT_MASK_KEY_PRESS | XCB_EVENT_MASK_KEY_RELEASE |
                        XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_POINTER_MOTION |
                        XCB_EVENT_MASK_STRUCTURE_NOTIFY;

        // Values to be sent over XCB (bg colour, events) <all black, our events>
        u32 value_list[] = {state->screen->black_pixel, event_values};

        // Creating the window, it's called a cookie for *obvious* reasons
        xcb_void_cookie_t cookie = xcb_create_window(
            state->connection,
            XCB_COPY_FROM_PARENT,
            state->window,
            state->screen->root,
            x, y, width, height,
            0, // border
            XCB_WINDOW_CLASS_INPUT_OUTPUT,
            state->screen->root_visual,
            event_mask,
            value_list);

        // set the window's title as it was not an option in the cookie creation
        xcb_change_property(
            state->connection,
            XCB_PROP_MODE_REPLACE,
            state->window,
            XCB_ATOM_WM_NAME,
            XCB_ATOM_STRING,
            8,  // data should be viewed 8 bits at a time
            strlen(wnd_name),
            wnd_name);
        

        // Tell the server to notify when the window manager
        // attempts to destroy the window.
        // Sort of Window Procedure that will respond to these protocols 
        xcb_intern_atom_cookie_t wm_delete_cookie = xcb_intern_atom(
            state->connection,
            0,
            strlen("WM_DELETE_WINDOW"),
            "WM_DELETE_WINDOW");
        xcb_intern_atom_cookie_t wm_protocols_cookie = xcb_intern_atom(
            state->connection,
            0,
            strlen("WM_PROTOCOLS"),
            "WM_PROTOCOLS");
        xcb_intern_atom_reply_t* wm_delete_reply = xcb_intern_atom_reply(
            state->connection,
            wm_delete_cookie,
            NULL);
        xcb_intern_atom_reply_t* wm_protocols_reply = xcb_intern_atom_reply(
            state->connection,
            wm_protocols_cookie,
            NULL);

        // Update struct so we can hook it up to our event system
        state->wm_delete_window = wm_delete_reply->atom;
        state->wm_protocols = wm_protocols_reply->atom;

        // Replaces Reply and Delete callbacks (?)
        // Pretend this makes sense...
        xcb_change_property(
            state->connection,
            XCB_PROP_MODE_REPLACE,
            state->window,
            wm_protocols_reply->atom,
            4,
            32,
            1,
            &wm_delete_reply->atom);

        // Finally, map window to screen
        xcb_map_window(state->connection, state->window);


        // Flush the stream
        // eveything buffered should be outputted
        i32 stream_result = xcb_flush(state->connection);

        // <= 0 is failure, so locked. Log fatal
        if (stream_result <= 0) {
            log->log_fatal() << "Failed to flush stream: " << stream_result;
            return FALSE;
        }

        return TRUE;
    }

    // TODO: Eventually get some memory subsystem upgrades
    // Handle allignment
    void *Platform::malloc(u64 sz, b8 alligned)
    {
        return std::malloc(sz);
    }

    void Platform::free(void *memory, b8 alligned)
    {
        std::free(memory);
    }
    
    void *Platform::memcpy(void *dest, const void *src, u64 sz)
    {
        return std::memcpy(dest, src, sz);
    }

    void *Platform::memzero(void *memory, u64 sz)
    {
        return std::memset(memory, 0, sz);

    }

    void *Platform::memset(void *memory, i32 value, u64 sz)
    {
        return std::memset(memory, value, sz);
    }

    
    b8 Platform::pump_message()
    {
        if (!abstract_wnd_state || !log)
        {
            // See init_windowing's note
            Logger tmp_logger;
            tmp_logger.log_fatal() << "Failed to init window. Constructor not called";
            return FALSE;
        }
        struct linux_windowing *state = (struct linux_windowing *)abstract_wnd_state;
        xcb_generic_event_t *event;

        xcb_client_message_event_t *client_message;
        b8 exit_flagged = FALSE;

        // TODO: Build input system
        // Poll for events until null is returned.
        while (event != 0)
        {
            event = xcb_poll_for_event(state->connection);

            if (event == 0)
            {
                break;
            }

            // Input events
            // convulted bitwise stuff based on the doc
            // pre-switch
            switch (event->response_type & ~0x80)
            {
                case XCB_KEY_PRESS:
                case XCB_KEY_RELEASE: {
                    /*
                    xcb_key_press_event_t *key_press_event;
                    key_press_event = (xcb_key_press_event_t *)event;
                    
                    b8 is_pressed = (event->response_type == XCB_KEY_PRESS);
                    xcb_keycode_t key_code = key_press_event->detail;

                    i32 shiftLevel = (key_code & ShiftMask) ? 1 : 0;

                    KeySym key_symbol = XkbKeycodeToKeysym(
                        state->display,
                        (KeyCode)key_code,
                        0,
                        shiftLevel 
                    );

                    enum keyboard key = translate_key(key_symbol);
                    input_handle_keyboard(key, is_pressed);
                    */

                } break;
                case XCB_BUTTON_PRESS:
                case XCB_BUTTON_RELEASE: {
                    /*
                    xcb_button_press_event_t *mouse_press_event;
                    mouse_press_event = (xcb_button_press_event_t *)event;

                    b8 is_pressed = (event->response_type == XCB_BUTTON_PRESS);
                    enum mouse button = MAX_MOUSE_BUTTONS;
                    switch (mouse_press_event->detail)
                    {
                        case XCB_BUTTON_INDEX_1:
                            button = MOUSE_LEFT;
                            break;
                        case XCB_BUTTON_INDEX_2:
                            button = MOUSE_MID;
                            break;
                        case XCB_BUTTON_INDEX_3:
                            button = MOUSE_RIGHT;
                            break;
                    }
                    if (button != MAX_MOUSE_BUTTONS)
                    {
                        input_handle_mouse(button, is_pressed);
                    }
                    */
                } break;
                case XCB_MOTION_NOTIFY: {
                    /*
                    xcb_motion_notify_event_t *mouse_move_event;
                    mouse_move_event = (xcb_motion_notify_event_t *)event;

                    input_handle_mouse_xy(mouse_move_event->event_x, mouse_move_event->event_y);
                    */
                } break;
                case XCB_CONFIGURE_NOTIFY: {
                    // TODO: Resizing from rendeder
                } break;
                case XCB_CLIENT_MESSAGE: {
                    client_message = (xcb_client_message_event_t*)event;
                    // Window close
                    if (client_message->data.data32[0] == state->wm_delete_window)
                    {
                        exit_flagged = TRUE;
                    }
                } break;
                default:
                    break;
            }

            // The thing is dynamic
            free(event, FALSE);
        }
        return !exit_flagged;
    }

    void Platform::sleep(u64 ms)
    {
    #if _POSIX_C_SOURCE >= 199309L
        struct timespec ts;
        ts.tv_sec = ms / 1000;
        ts.tv_nsec = (ms % 1000) * 1000 * 1000;
        nanosleep(&ts, 0);
    #else
        if (ms >= 1000) {
            sleep(ms / 1000);
        }
        usleep((ms % 1000) * 1000);
    #endif
    }

}

#endif