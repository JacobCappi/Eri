#pragma once

#include "defines.h"

// Interface for platform layer
// Currently exported functions should not be, they are only exported for testing..

// Pointer to the internal state
typedef struct platform_state
{
    void *os_specific_state;
} platform_state;

ERI_API b8 platform_startup(
    platform_state *state,
    const char *name,
    i32 x, i32 y,
    i32 width, i32 height);

// ----- State Management
ERI_API void platform_shutdown(platform_state *state);
ERI_API b8 platform_message(platform_state *state);
// ----- END

// ----- Platform based Memory Management
void *platform_malloc(u64 size, b8 aligned);
void platform_free(void *memory, b8 aligned);
void *platform_memzero(void *memory, u64 size);
void *platform_memcpy(void *source, const void *destination, u64 size);
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