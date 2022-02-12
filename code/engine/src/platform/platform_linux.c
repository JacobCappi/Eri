#include "platform/platform.h"

#if ERI_PLATFORM_LINUX 

#include "core/event.h"
#include "core/input.h"
#include "core/logger.h"

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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// xlib and xcb concept from KOHI
// Honestly, linux has too many choices, and the
// documentation is pretty bad.
// I'll go with what the professional did with
// linux platform layer
struct linux_state 
{
    Display *display; // xLib portion
    xcb_connection_t *connection;
    xcb_window_t window;
    xcb_screen_t *screen;
    xcb_atom_t wm_protocols;
    xcb_atom_t wm_delete_window;
};

enum keyboard translate_key(u32 keys);

b8 init_platform(
    struct platform_state *state,
    const char *name,
    i32 x, i32 y,
    i32 width, i32 height
)
{
    ERI_LOG_INFO("Initializing Subsystem: Platform Type [ Linux ]");
    state->os_specific_state = platform_malloc(sizeof(struct linux_state), FALSE);
    struct linux_state *os_state = (struct linux_state *)state->os_specific_state;

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
    // TODO: this makes 0 sense... what is this?
    int screen_p = 0;
    for (i32 s = screen_p; s > 0; s--) {
        xcb_screen_next(&screen_iterator);
    }
    os_state->screen = screen_iterator.data;

    os_state->window = xcb_generate_id(os_state->connection);

    // Must register events, before being able to listen, so KOHI
    // just enabled all of the ones we'll be needing right here.

    // Register event types.
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

    // Window creation returns a 'cookie', and it is what it is....
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
    // throughout this code
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

void shutdown_platform(struct platform_state *state)
{
    ERI_LOG_INFO("Shutting Down Subsystem: Platform Layer [ Linux ]");
    struct linux_state *os_state = (struct linux_state *)state->os_specific_state;
    // TODO: uncomment
    //XAutoRepeatOn(os_state->display);
    xcb_destroy_window(os_state->connection ,os_state->window);
}

b8 platform_message(struct platform_state *state)
{
    // similiar to win32 platform message pumping
    struct linux_state *os_state = (struct linux_state *)state->os_specific_state;
    xcb_generic_event_t *event;

    xcb_client_message_event_t *client_message;
    b8 exit_flagged = FALSE;

    // Poll for events until null is returned.
    while (event != 0)
    {
        event = xcb_poll_for_event(os_state->connection);

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
                xcb_key_press_event_t *key_press_event;
                key_press_event = (xcb_key_press_event_t *)event;
                
                b8 is_pressed = (event->response_type == XCB_KEY_PRESS);
                xcb_keycode_t key_code = key_press_event->detail;

                i32 shiftLevel = (key_code & ShiftMask) ? 1 : 0;

                KeySym key_symbol = XkbKeycodeToKeysym(
                    os_state->display,
                    (KeyCode)key_code,
                    0,
                    shiftLevel 
                );

                enum keyboard key = translate_key(key_symbol);
                input_handle_keyboard(key, is_pressed);

            } break;
            case XCB_BUTTON_PRESS:
            case XCB_BUTTON_RELEASE: {
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
            } break;
            case XCB_MOTION_NOTIFY: {
                xcb_motion_notify_event_t *mouse_move_event;
                mouse_move_event = (xcb_motion_notify_event_t *)event;

                input_handle_mouse_xy(mouse_move_event->event_x, mouse_move_event->event_y);
            } break;
            case XCB_CONFIGURE_NOTIFY: {
                // TODO: Resizing
            } break;
            case XCB_CLIENT_MESSAGE: {
                client_message = (xcb_client_message_event_t*)event;
                // Window close
                if (client_message->data.data32[0] == os_state->wm_delete_window)
                {
                    exit_flagged = TRUE;
                }
            } break;
            default:
                break;
        }

        // The thing is dynamic
        free(event);
    }
    return !exit_flagged;
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

void *platform_memcpy(void *destination, const void *source, u64 size)
{
    return memcpy(destination, source, size);
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

f64 platform_time(void)
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

enum keyboard translate_key(u32 keys)
{
    // This took an hour.... respect it
    switch (keys)
    {
        // TODO: Change this.. handle each key, not shift-key ... causes bugs
        case XK_space:
            return KEY_SPACE;
        case XK_quotedbl:
        case XK_apostrophe:
            return KEY_QUOTE;
        case XK_plus:
        case XK_equal:
            return KEY_EQUAL;
        case XK_comma:
        case XK_less:
            return KEY_COMMA;
        case XK_minus:
        case XK_underscore:
            return KEY_MINUS;
        case XK_period:
        case XK_greater:
            return KEY_PERIOD;
        case XK_question:
        case XK_slash:
            return KEY_FSLASH;

        case XK_parenright:
        case XK_0:
            return KEY_0;
        case XK_exclam:
        case XK_1:
            return KEY_1;
        case XK_at:
        case XK_2:
            return KEY_2;
        case XK_numbersign:
        case XK_3:
            return KEY_3;
        case XK_dollar:
        case XK_4:
            return KEY_4;
        case XK_percent:
        case XK_5:
            return KEY_5;
        case XK_asciicircum:
        case XK_6:
            return KEY_6;
        case XK_ampersand:
        case XK_7:
            return KEY_7;
        case XK_multiply:
        case XK_8:
            return KEY_8;
        case XK_parenleft:
        case XK_9:
            return KEY_9;
        case XK_A:
        case XK_a:
            return KEY_A;
        case XK_B:
        case XK_b:
            return KEY_B;
        case XK_C:
        case XK_c:
            return KEY_C;
        case XK_D:
        case XK_d:
            return KEY_D;
        case XK_E:
        case XK_e:
            return KEY_E;
        case XK_F:
        case XK_f:
            return KEY_F;
        case XK_G:
        case XK_g:
            return KEY_G;
        case XK_H:
        case XK_h:
            return KEY_H;
        case XK_I:
        case XK_i:
            return KEY_I;
        case XK_J:
        case XK_j:
            return KEY_J;
        case XK_K:
        case XK_k:
            return KEY_K;
        case XK_L:
        case XK_l:
            return KEY_L;
        case XK_M:
        case XK_m:
            return KEY_M;
        case XK_N:
        case XK_n:
            return KEY_N;
        case XK_O:
        case XK_o:
            return KEY_O;
        case XK_P:
        case XK_p:
            return KEY_P;
        case XK_Q:
        case XK_q:
            return KEY_Q;
        case XK_R:
        case XK_r:
            return KEY_R;
        case XK_S:
        case XK_s:
            return KEY_S;
        case XK_T:
        case XK_t:
            return KEY_T;
        case XK_U:
        case XK_u:
            return KEY_U;
        case XK_V:
        case XK_v:
            return KEY_V;
        case XK_W:
        case XK_w:
            return KEY_W;
        case XK_X:
        case XK_x:
            return KEY_X;
        case XK_Y:
        case XK_y:
            return KEY_Y;
        case XK_Z:
        case XK_z:
            return KEY_Z;
        case XK_braceleft:
        case XK_bracketleft:
            return KEY_LBRACKET;
        case XK_braceright:
        case XK_bracketright:
            return KEY_RBRACKET;
        case XK_asciitilde:
        case XK_grave:
            return KEY_GRAVE;
        
        case XK_BackSpace:
            return KEY_BACKSPACE;
        case XK_Tab:
            return KEY_TAB;
        case XK_Return:
            return KEY_ENTER;
        case XK_Pause:
            return KEY_PAUSE;
        case XK_Scroll_Lock:
            return KEY_SCRLLK;
        case XK_Escape:
            return KEY_ESCAPE;
        case XK_Home:
            return KEY_HOME;
        case XK_Left:
            return KEY_LEFT;
        case XK_Up:
            return KEY_UP;
        case XK_Right:
            return KEY_RIGHT;
        case XK_Down:
            return KEY_DOWN;
        case XK_Prior:
            return KEY_PGUP;
        case XK_Next:
            return KEY_PGDWN;
        case XK_End:
            return KEY_END;
        case XK_Select:
            return KEY_SELECT;
        case XK_Print:
            return KEY_PRINT;
        case XK_Insert:
            return KEY_INSERT;

        case XK_KP_Enter:
            return KEY_KP_ENTER;
        case XK_KP_Multiply:
            return KEY_KP_MULT;
        case XK_KP_Add:
            return KEY_KP_ADD;
        case XK_KP_Subtract:
            return KEY_KP_SUB;
        case XK_KP_Decimal:
            return KEY_KP_DOT;
        case XK_KP_Divide:
            return KEY_KP_DIV;

        case XK_KP_0:
            return KEY_KP_0;
        case XK_KP_1:
            return KEY_KP_1;
        case XK_KP_2:
            return KEY_KP_2;
        case XK_KP_3:
            return KEY_KP_3;
        case XK_KP_4:
            return KEY_KP_4;
        case XK_KP_5:
            return KEY_KP_5;
        case XK_KP_6:
            return KEY_KP_6;
        case XK_KP_7:
            return KEY_KP_7;
        case XK_KP_8:
            return KEY_KP_8;
        case XK_KP_9:
            return KEY_KP_9;

        case XK_F1:
            return KEY_F1;
        case XK_F2:
            return KEY_F2;
        case XK_F3:
            return KEY_F3;
        case XK_F4:
            return KEY_F4;
        case XK_F5:
            return KEY_F5;
        case XK_F6:
            return KEY_F6;
        case XK_F7:
            return KEY_F7;
        case XK_F8:
            return KEY_F8;
        case XK_F9:
            return KEY_F9;
        case XK_F10:
            return KEY_F10;
        case XK_F11:
            return KEY_F11;
        case XK_F12:
            return KEY_F12;
        case XK_F13:
            return KEY_F13;
        case XK_F14:
            return KEY_F14;
        case XK_F15:
            return KEY_F15;
        case XK_F16:
            return KEY_F16;
        case XK_F17:
            return KEY_F17;
        case XK_F18:
            return KEY_F18;
        case XK_F19:
            return KEY_F19;
        case XK_F20:
            return KEY_F20;
        case XK_F21:
            return KEY_F21;
        case XK_F22:
            return KEY_F22;
        case XK_F23:
            return KEY_F23;
        case XK_F24:
            return KEY_F24;

        case XK_Shift_L:
        case XK_Shift_R:
            return KEY_SHIFT;
        case XK_Control_L:
        case XK_Control_R:
            return KEY_CONTROL;
        case XK_Caps_Lock:
            return KEY_CAPSLK;
        case XK_Alt_L:
        case XK_Alt_R:
            return KEY_ALT;
        case XK_Super_L:
        case XK_Super_R:
            return KEY_SUPER;
        case XK_Delete:
            return KEY_DELETE;

        default:
            return 0;
    }
}

#endif