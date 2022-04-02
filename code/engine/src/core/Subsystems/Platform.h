#pragma once
#include "defines.h"
#include "core/Logger/Logger.h"
#include "core/Subsystems/ISubsystem.h"

//TODO: let get_name return custom string lib
#include <string>


namespace ERI
{
    class Platform : public ISubsystem
    {

    private:
        std::string subsystem_name = "Unknown";
        void *abstract_wnd_state = nullptr;
        Logger *log = nullptr;

    protected:
        void set_subsystem_name(const std::string name);

    public:
        Platform() = default;
        ~Platform() = default;

    //TODO: consider shared pointers?
        b8 init(Logger *log);
        void shutdown(void);
        const std::string get_name(void);

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