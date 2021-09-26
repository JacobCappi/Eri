#include "platform/platform.h"

// OS Check to compile proper implementation of interface
#if ERI_PLATFORM_LINUX 

// (libs) libx11-dev, libxkbcommon-x11-dev 
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

// Everything needed for xcb window to open
typedef struct linux_state 
{
    Display *display;
    xcb_connection_t *connection;
    xcb_window_t *window;
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
}

void platform_shutdown(platform_state *state);
b8 platform_message(platform_state *state);

void *platform_malloc(u64 size, b8 aligned);
void platform_free(void *memory, b8 aligned);
void *platform_memzero(void *memory, u64 size);
void *platform_memcpy(void *end, const void *start, u64 size);
void *platform_memset(void *memory, i32 value, u64 size);

void platform_cout(const char *text, u8 color);
void platform_cerr(const char *text, u8 color);

f64 platform_time();
void platform_sleep(u64 ms);

#endif