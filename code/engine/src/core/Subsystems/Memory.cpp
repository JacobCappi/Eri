#include "core/Subsystems/Memory.h"

namespace ERI
{
    b8 Memory::init(Logger *log)
    {
        subsystem_name = new String("[ Memory Subsystem ] ");
    }

    void Memory::shutdown(void)
    {
        delete subsystem_name;
    }

    String Memory::get_name(void)
    {
        return *subsystem_name;
    }

    b8 Memory::print_stats()
    {
        log->log_debug() << subsystem_name << "Total Alloc: " << total_alloc << "\n";
        for (u16 i = 0; i < MAX_MEM_TAGS; i++)
        {
            log->log_debug() << "\t" << mem

        }
    }

}