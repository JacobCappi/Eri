#pragma once

#include "defines.h"

struct platform_state
{
    void *os_specific_state;
};

// ----- Start State Management
b8 init_platform(
    struct platform_state *state,
    const char *name,
    i32 x, i32 y,
    i32 width, i32 height);
void shutdown_platform(struct platform_state *state);
// ----- END

b8   platform_message(struct platform_state *state);

// TODO: Alignment
// ----- Start Platform based Memory Management
void *platform_malloc(u64 size, b8 aligned);
void  platform_free(void *memory, b8 aligned);
void *platform_memzero(void *memory, u64 size);
void *platform_memcpy(void *destination, const void *source, u64 size);
void *platform_memset(void *memory, i32 value, u64 size);
// ----- END

// ----- Start Platform based i/o
void platform_cout(const char *text, u8 color);
void platform_cerr(const char *text, u8 color);
// ----- END

// ------ Start Platform based OS commands
f64  platform_time(void);
void platform_sleep(u64 ms);
// ----- END