#pragma once
#include "defines.h"
#include "core/Logger/Logger.h"


// TODO: Subject to change(?)
// What migrates and what doesn't in cpp xserver
namespace ERI
{

    class Platform
    {

    public:
    // Testing dependency injection :/
        Platform(Logger logger);
        ~Platform();

    private:
        Logger *log;
        void *abstract_wnd_state = nullptr;


    public:
        bool init_windowing(const char *wnd_name, i32 x, i32 y, i32 width, i32 height);
        b8 pump_message();

        void *malloc(u64 sz, b8 alligned);
        void free(void *memory, b8 alligned);
        void *memcpy(void *dest, const void *src, u64 sz);
        void *memzero(void *memory, u64 sz);
        void *memset(void *memory, i32 value, u64 sz);

        f64 current_time(void);
        void sleep(u64 ms);

        // TODO: input layer
        // virtual enum keyboard translate_key(u32 keys)
    };

}