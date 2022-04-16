#pragma once

#include <vector>

#include "core/DataStructs/Strings.h"
#include "core/Logger/Logger.h"
#include "core/Subsystems/ISubsystem.h"


namespace ERI
{
    class Memory : protected ISubsystem
    {

    private:
        String *subsystem_name;
        Logger *log = nullptr;

    private:
        enum tag
        {
            // Should only be temp use 
            MEM_UNKNOWN,

            MEM_ARRAY,
            MEM_DARRAY,
            MEM_CARRAY,
            MEM_STRING,

            MEM_DICT,
            MEM_BST,

            MEM_APPLICATION,
            MEM_GAME,
            MEM_JOB,

            MEM_TEXTURE,
            MEM_MATERIAL_INSTANCE,
            MEM_RENDERER,
            MEM_TRANSFORM,

            MEM_ENTITY,
            MEM_ENTITY_NODE,
            MEM_SCENE,

            MAX_MEM_TAGS 
        };

        String mem_tag_strings[MAX_MEM_TAGS] =
        {
            "MEM_UNKNOWN",

            "MEM_ARRAY",
            "MEM_DARRAY",
            "MEM_CARRAY",
            "MEM_STRING",

            "MEM_DICT",
            "MEM_BST",

            "MEM_APPLICATION",
            "MEM_GAME",
            "MEM_JOB",

            "MEM_TEXTURE",
            "MEM_MATERIAL_INSTANCE",
            "MEM_RENDERER",
            "MEM_TRANSFORM",

            "MEM_ENTITY",
            "MEM_ENTITY_NODE",
            "MEM_SCENE"
        };


    private:
        u64 total_alloc;
        u64 usage_by_tag[MAX_MEM_TAGS];

    public:
        Memory() = default;
        ~Memory() = default;

        b8 init(Logger *log);
        void shutdown(void);
        String get_name(void);
        b8 print_stats(void);
    };

}