#pragma once

#include "defines.h"

// Interface for platform layer

// Pointer to the internal state
typedef struct platform_state
{
    void *specified_state;
} platform_state;

b8 platform_startup(
    platform_state *state,
    const char *name,
    i32 x, i32 y,
    i32 width, i32 height
);

// ----- State Management
void platform_shutdown(platform_state *state);
b8 platform_message(platform_state *state);
// ----- END

// ----- Wrapper for Memory 
void *platform_malloc(u64 size, b8 aligned);
void platform_free(void *memory, b8 aligned);
void *platform_memzero(void *memory, u64 size);
void *platform_memcpy(void *end, const void *start, u64 size);
void *platform_memset(void *memory, i32 value, u64 size);
// ----- END

// ----- Platform based color console outs
void platform_cout(const char *text, u8 color);
void platform_cerr(const char *text, u8 color);
// ----- END

// ------ Platform based OS commands
f64 platform_time();
void platform_sleep(u64 ms);
// ----- END