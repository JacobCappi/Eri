#include "core/input.h"

#include "core/event.h"
#include "core/logger.h"
#include "core/mem.h"

struct keyboard_state
{
    b8 keys[MAX_KEYS];
};

struct mouse_state
{
    i16 x;
    i16 y;
    b8 buttons[MAX_MOUSE_BUTTONS];
};

struct input_state
{
    struct keyboard_state keyboard_new;
    struct keyboard_state keyboard_old;

    struct mouse_state    mouse_new;
    struct mouse_state    mouse_old;
};

static b8 is_init = FALSE;
static struct input_state input_system;

b8 init_input()
{
    ERI_LOG_INFO("Initializing Subsystem: [ Input  ]");

    if (is_init == TRUE)
    {
        ERI_LOG_WARNING("Input Subsystem is already initialized");
        return FALSE;
    }

    eri_memzero(&input_system, sizeof(struct input_state));
    is_init = TRUE;
    return TRUE;
}

void shutdown_input()
{
    // TODO: shutdown routines
    is_init = FALSE;
}

void input_update(f64 delta_time)
{
    if (is_init == FALSE)
    {
        return;
    }
    eri_memcpy(&input_system.keyboard_old, &input_system.keyboard_new, sizeof(struct keyboard_state));
    eri_memcpy(&input_system.mouse_old, &input_system.mouse_new, sizeof(struct mouse_state));
}

b8 input_is_key_down(enum keyboard key)
{
    if (is_init == FALSE)
    {
        return FALSE;
    }

    return input_system.keyboard_new.keys[key] == TRUE;
}

b8 input_was_key_down(enum keyboard key)
{
    if (is_init == FALSE)
    {
        return FALSE;
    }

    return input_system.keyboard_old.keys[key] == TRUE;
}

b8 input_is_key_up(enum keyboard key)
{
    if (is_init == FALSE)
    {
        return TRUE;
    }

    return input_system.keyboard_new.keys[key] == FALSE;
}

b8 input_was_key_up(enum keyboard key)
{
    if (is_init == FALSE)
    {
        return TRUE;
    }

    return input_system.keyboard_old.keys[key] == FALSE;
}

b8 input_is_button_down(enum mouse button)
{
    if (is_init == FALSE)
    {
        return FALSE;
    }

    return input_system.mouse_new.buttons[button] == TRUE;
}

b8 input_was_button_down(enum mouse button)
{
    if (is_init == FALSE)
    {
        return FALSE;
    }

    return input_system.mouse_old.buttons[button] == TRUE;
}

b8 input_is_button_up(enum mouse button)
{
    if (is_init == FALSE)
    {
        return TRUE;
    }

    return input_system.mouse_new.buttons[button] == FALSE;

}

b8 input_was_button_up(enum mouse button)
{
    if (is_init == FALSE)
    {
        return TRUE;
    }

    return input_system.mouse_old.buttons[button] == FALSE;
}

void input_new_mouse_xy(i32 *x, i32 *y)
{
    if (is_init == FALSE)
    {
        *x = 0;
        *y = 0;
        return;
    }
    *x = input_system.mouse_new.x;
    *y = input_system.mouse_new.y;
}

void input_old_mouse_xy(i32* x, i32* y)
{
    if (is_init == FALSE)
    {
        *x = 0;
        *y = 0;
        return;
    }

    *x = input_system.mouse_old.x;
    *y = input_system.mouse_old.y;
}

void input_handle_keyboard(enum keyboard key, b8 pressed)
{
    if (input_system.keyboard_new.keys[key] != pressed)
    {
        input_system.keyboard_new.keys[key] = pressed;

        struct event_args event_data;
        event_data.u16[0] = key;
        event_raise(pressed ? EVENT_KEY_PRESSED : EVENT_KEY_RELEASED, 0, event_data);
    }
}

void input_handle_mouse(enum mouse button, b8 pressed)
{
    ERI_LOG_DEBUG("Mouse Button %d pressed", button);
    if (input_system.mouse_new.buttons[button] != pressed)
    {
        input_system.mouse_new.buttons[button] = pressed;

        struct event_args event_data;
        event_data.u16[0] = button;
        event_raise(pressed ? EVENT_MOUSE_PRESSED : EVENT_MOUSE_RELEASED, 0, event_data);
    }
}

void input_handle_mouse_xy(i16 x, i16 y)
{
    ERI_LOG_DEBUG("Mouse x: %d, y: %d", x, y);
    if (input_system.mouse_new.x != x || input_system.mouse_new.y != y)
    {
        input_system.mouse_new.x = x;
        input_system.mouse_new.y = y;

        struct event_args event_data;
        event_data.u16[0] = x;
        event_data.u16[0] = y;
        event_raise(EVENT_MOUSE_MOVED, 0, event_data);
    }
}

void input_handle_mouse_wheel(i8 z_delta)
{
    struct event_args event_data;
    event_data.u8[0] = z_delta;
    event_raise(EVENT_MOUSE_WHEEL, 0, event_data);
}
