#pragma once
// Note: Memory.h was taken, so mem.h
#include "defines.h"

enum mem_tag
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

// ----- Subsystem Handling
b8 init_memory(void);
void shutdown_memory(void);
// ----- END


// Returns char* of mem usage
ERI_API char* mem_get_status(void);

// Note: Free contains size parameter for engine to update mem use
ERI_API void  eri_free(void *memory, u64 size, enum mem_tag tag);
ERI_API void *eri_malloc(u64 size, enum mem_tag tag);

ERI_API void *eri_memset(void *memory, i32 value, u64 size);
ERI_API void *eri_memcpy(void *destination, const void *source, u64 size);
ERI_API void *eri_memzero(void *memory, u64 size);
