#include "platform/platform.h"

// OS Check to compile proper implementation of interface
#if ERI_PLATFORM_WIN32

#include <windows.h>
#include <windowsx.h>

b8 platform_startup(
    platform_state *state,
    const char *name,
    i32 x, i32 y,
    i32 width, i32 height
){
    return TRUE;
}

void platform_shutdown(platform_state *state);
b8 platform_message(platform_state *state);

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

void *platform_memcpy(void *end, const void *start, u64 size)
{
    return memcpy(end, start, size);
}

void *platform_memset(void *memory, i32 value, u64 size)
{
    return memset(memory, value, size);
}

void platform_cout(const char *text, u8 color);
void platform_cerr(const char *text, u8 color);

f64 platform_time();
void platform_sleep(u64 ms);

#endif