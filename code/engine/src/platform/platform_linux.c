#include "platform/platform.h"

// OS Check to compile proper implementation of interface
#if ERI_PLATFORM_LINUX 

#include "core/logger.h"

// (libs) libx11-dev, libxkbcommon-x11-dev 
// xcb is a newer lib
#include <xcb/xcb.h>
#include <X11/keysym.h>
#include <X11/XKBlib.h> 
#include <X11/Xlib.h>
#include <X11/Xlib-xcb.h> 
#include <sys/time.h>

// Some distros use different libs
// nanosleep vs usleep
#if _POSIX_C_SOURCE >= 199309L
    #include <time.h>
#else
    #include <unistd.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// xlib and xcb concept from KOHI
// Honestly, linux has too many choices, and the
// documentation is even worse than msdn's win32, so 
// I'll go with what the professional did with
// linux platform layer
typedef struct linux_state 
{
    Display *display; // xLib portion
    xcb_connection_t *connection;
    xcb_window_t window;
    xcb_screen_t *screen;
    xcb_atom_t wm_protocols;
    xcb_atom_t wm_delete_window;
} linux_state;

b8 platform_startup(
    platform_state *state,
    const char *name,
    i32 x, i32 y,
    i32 width, i32 height
)
{
    state->os_specific_state = platform_malloc(sizeof(linux_state), FALSE);
    linux_state *os_state = (linux_state *)state->os_specific_state;

    // Connect to the X Server ( the only xlib call)
    os_state->display = XOpenDisplay(NULL);

    // NOTE: Disables AutoRepeatOff of the Xserver...
    // the one that is on when you startx, so it does
    // globally disable it. If I forget to turn this back on,
    // you have to reboot.

    // NOTE: xset r $state is a command you can run if the close event never fired
    // TODO: uncomment
    // XAutoRepeatOff(os_state->display);
    os_state->connection = XGetXCBConnection(os_state->display);

    if (xcb_connection_has_error(os_state->connection))
    {
        ERI_LOG_FATAL("XCB failed to connect to X11 server");
        return FALSE;
    }

    // Get's info from xserver
    const struct xcb_setup_t *xcb_setup = xcb_get_setup(os_state->connection);

    // Then we *have* to iteratate through available screens to be able to set the screen
    xcb_screen_iterator_t screen_iterator = xcb_setup_roots_iterator(xcb_setup);
    int screen_p = 0;
    for (i32 s = screen_p; s > 0; s--) {
        xcb_screen_next(&screen_iterator);
    }
    os_state->screen = screen_iterator.data;

    os_state->window = xcb_generate_id(os_state->connection);

    // Must register events, before being able to listen, so KOHI
    // just enabled all of the ones we'll be needing right here.

    // Register event types.
    // XCB_CW_BACK_PIXEL = filling then window bg with a single colour
    // XCB_CW_EVENT_MASK is required.
    u32 event_mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;

    // Listen for keyboard and mouse buttons
    u32 event_values = XCB_EVENT_MASK_BUTTON_PRESS | XCB_EVENT_MASK_BUTTON_RELEASE |
                       XCB_EVENT_MASK_KEY_PRESS | XCB_EVENT_MASK_KEY_RELEASE |
                       XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_POINTER_MOTION |
                       XCB_EVENT_MASK_STRUCTURE_NOTIFY;

    // Values to be sent over XCB (bg colour, events)
    // In this case, (black it out, and enable all these events)
    u32 value_list[] = {os_state->screen->black_pixel, event_values};

    // Window creation returns a 'cookie', and it is what it is.
    xcb_void_cookie_t cookie = xcb_create_window(
        os_state->connection,
        XCB_COPY_FROM_PARENT, // depth?
        os_state->window,
        os_state->screen->root, // the parent screen?
        x, y, width, height,
        0, // border
        XCB_WINDOW_CLASS_INPUT_OUTPUT,  // class [accepts input and also output events]
        os_state->screen->root_visual, // visual?
        event_mask, // our event bois
        value_list);
    
    // set the window's title as it was not an option in the cookie creation
    xcb_change_property(
        os_state->connection,
        XCB_PROP_MODE_REPLACE,
        os_state->window,
        XCB_ATOM_WM_NAME,
        XCB_ATOM_STRING,
        8,  // data should be viewed 8 bits at a time
        strlen(name),
        name);

    // Tell the server to notify when the window manager
    // attempts to destroy the window.
    // Sort of Window Procedure that will respond to these protocols 
    xcb_intern_atom_cookie_t wm_delete_cookie = xcb_intern_atom(
        os_state->connection,
        0,
        strlen("WM_DELETE_WINDOW"),
        "WM_DELETE_WINDOW");
    xcb_intern_atom_cookie_t wm_protocols_cookie = xcb_intern_atom(
        os_state->connection,
        0,
        strlen("WM_PROTOCOLS"),
        "WM_PROTOCOLS");
    xcb_intern_atom_reply_t* wm_delete_reply = xcb_intern_atom_reply(
        os_state->connection,
        wm_delete_cookie,
        NULL);
    xcb_intern_atom_reply_t* wm_protocols_reply = xcb_intern_atom_reply(
        os_state->connection,
        wm_protocols_cookie,
        NULL);

    // A copy being stored in the struct
    os_state->wm_delete_window = wm_delete_reply->atom;
    os_state->wm_protocols = wm_protocols_reply->atom;

    // Replaces Reply and Delete callbacks (?)
    // With minimal documentation, Kohi also struggled
    // throught his code
    xcb_change_property(
        os_state->connection,
        XCB_PROP_MODE_REPLACE,
        os_state->window,
        wm_protocols_reply->atom,
        4,
        32,
        1,
        &wm_delete_reply->atom);

    // Map the window to the screen
    xcb_map_window(os_state->connection, os_state->window);


    // Flush the stream
    // eveything buffered should be outputted
    i32 stream_result = xcb_flush(os_state->connection);

    // <= 0 is failure, so locked. Log fatal
    if (stream_result <= 0) {
        ERI_LOG_FATAL("Failed to flush the stream: %d", stream_result);
        return FALSE;
    }

    return TRUE;
}

void platform_shutdown(platform_state *state)
{
    linux_state *os_state = (linux_state *)state->os_specific_state;
    // TODO: uncomment
    //XAutoRepeatOn(os_state->display);
    xcb_destroy_window(os_state->connection ,os_state->window);
}

b8 platform_message(platform_state *state)
{
    // similiar to win32 platform message pumping
    linux_state *os_state = (linux_state *)state->os_specific_state;
    xcb_generic_event_t* event;

    xcb_client_message_event_t* c_message;
    b8 quit_flagged = FALSE;

    // Poll for events until null is returned.
    while (event != 0) {
        event = xcb_poll_for_event(os_state->connection);
        if (event == 0) {
            break;
        }

        // Input events
        // convulted bitwise stuff based on the doc
        // pre-switch
        switch (event->response_type & ~0x80) {
            case XCB_KEY_PRESS:
            case XCB_KEY_RELEASE: {
                // TODO: Key presses and releases
            } break;
            case XCB_BUTTON_PRESS:
            case XCB_BUTTON_RELEASE: {
                // TODO: Mouse button presses and releases
            } break;
            case XCB_MOTION_NOTIFY:
                // TODO: mouse movement
                break;

            case XCB_CONFIGURE_NOTIFY: {
                // TODO: Resizing
            } break;

            case XCB_CLIENT_MESSAGE: {
                c_message = (xcb_client_message_event_t*)event;
                // Window close
                if (c_message->data.data32[0] == os_state->wm_delete_window) {
                    quit_flagged = TRUE;
                }
            } break;
            default:
                // Something else
                break;
        }

        // The thing is dynamic
        free(event);
    }
    return !quit_flagged;
}

// TODO: Memory systems: currently temp use of stdlib
void *platform_malloc(u64 size, b8 aligned)
{
    return malloc(size);
}

void platform_free(void *memory, b8 aligned)
{
    free(memory);
}

void *platform_memzero(void *memory, u64 size)
{
    return memset(memory, 0, size);
}

void *platform_memcpy(void *source, const void *destination, u64 size)
{
    return memcpy(source, destination, size);
}

void *platform_memset(void *memory, i32 value, u64 size)
{
    return memset(memory, value, size);
}

// unlike windows, ansi is enabled by default, so just printf
void platform_cout(const char *text, u8 color)
{
    // Fatal, Error, Warning, Info, Debug, Trace
    const char* color_strings[] = {"0;41", "1;31", "1;33", "1;32", "1;34", "1;30"};
    printf("\033[%sm%s\033[0m", color_strings[color], text);
}

void platform_cerr(const char *text, u8 color)
{
    // TODO: maybe error stream
    // Fatal, Error, Warning, Info, Debug, Trace
    const char* color_strings[] = {"0;41", "1;31", "1;33", "1;32", "1;34", "1;30"};
    printf("\033[%sm%s\033[0m", color_strings[color], text);
}

f64 platform_time()
{
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    return now.tv_sec + now.tv_nsec * 0.000000001;
}

void platform_sleep(u64 ms)
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

#endif
