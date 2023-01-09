#pragma once
#include "defines.h"

#include "core/DataStructs/Strings.h"
#include "core/Logger/Logger.h"
#include "core/Subsystems/ISubsystem.h"


namespace ERI
{
    class Platform : public ISubsystem
    {

    private:
        String *subsystem_name = nullptr;
        void *abstract_wnd_state = nullptr;
        Logger *log = nullptr;

    public:
        Platform() = default;
        ~Platform() = default;

        String get_name(void);

    //TODO: consider shared pointers?
        b8 init(Logger *log);
        void shutdown(void);

        b8 init_windowing(const char *wnd_name, i32 x, i32 y, i32 width, i32 height);
        b8 pump_message();

        void *malloc(u64 sz, b8 alligned);
        void free(void *memory, b8 alligned);
        void *memcpy(void *dest, const void *src, u64 sz);
        void *memzero(void *memory, u64 sz);
        void *memset(void *memory, i32 value, u64 sz);

        void sleep(u64 ms);

        // TODO: input layer
        // virtual enum keyboard translate_key(u32 keys)
    };

}