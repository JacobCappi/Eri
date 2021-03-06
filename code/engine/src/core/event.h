#pragma once

#include "defines.h"

struct event_args
{
    // Max of 128 bits for event args 
    union
    {
        i64 i64[2];
        i32 i32[4];
        i16 i16[8];
        i8  i8[16];

        u64 u64[2];
        u32 u32[4];
        u16 u16[8];
        u8  u8[16];

        f64 f64[2];
        f32 f32[4];

        char str[16];
    };
};

// KOHI design concept: 0-255 internal, 255+ application
enum internal_system_event_codes {

    // Shuts the application down on the next frame.
    EVENT_APPLICATION_QUIT = 0x01,

    EVENT_KEY_PRESSED      = 0x02,
    EVENT_KEY_RELEASED     = 0x03,

    EVENT_MOUSE_PRESSED    = 0x04,
    EVENT_MOUSE_RELEASED   = 0x05,
    EVENT_MOUSE_MOVED      = 0x06,
    EVENT_MOUSE_WHEEL      = 0x07,

    EVENT_WINDOW_RESIZE    = 0x08,

    MAX_EVENT_CODE         = 0xFF
}; 

// ----- Subsystem handling
b8 init_event(void);
void shutdown_event(void);
// ----- END

typedef b8 (*on_raised_event)(u16 event_code, void *publisher, void *subsciber_instance, struct event_args data);

// Hooks up / Removes subscriber to on raised event function pointer
ERI_API b8 event_subscribe(u16 event_code, void *subscriber, on_raised_event callback);
ERI_API b8 event_unsubscribe(u16 event_code, void *subscriber, on_raised_event callback);
ERI_API b8 event_raise(u16 event_code, void *publisher, struct event_args data);
// ----- END