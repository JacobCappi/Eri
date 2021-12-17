#include "core/mem.h"

#include "core/logger.h"
#include "platform/platform.h"

// TODO: Custom String lib (?)
#include "string.h"
#include "stdio.h"

struct mem_stats
{
    u64 total_allocated;
    u64 mem_by_tag[MEM_MAX_TAGS];
};

// TODO: more robust solution
// Should be one to one with the enum
static const char* mem_tag_headers[MEM_MAX_TAGS] = 
{
    "UNKNOWN     ",
    "ARRAY       ",
    "DARRAY      ",
    "CARRAY      ",
    "STRING      ",
    "DICT        ",
    "BST         ",
    "APPLICATION ",
    "GAME        ",
    "JOB         ",
    "TEXTURE     ",
    "MAT_INST    ",
    "RENDERER    ",
    "TRANSFORM   ",
    "ENTITY      ",
    "ENTITY_NODE ",
    "SCENE       "
};

static struct mem_stats stats;

void init_memory(void)
{
    ERI_LOG_INFO("Eri initialized memory subsystem...");
    platform_memzero(&stats, sizeof(stats));
}

void shutdown_memory(void)
{
    // TODO: Assign shutdown if I ever have something to cleanup
    ERI_LOG_INFO("Eri shutdown memory subsystem...");
    return;
}

// Debug function to output all malloc totals by tag
char* get_mem_status(void)
{
    ERI_LOG_INFO("Debug function \"get_mem_status\" called");
    const u64 GB = 1024 * 1024 * 1024;
    const u64 MB = 1024 * 1024;
    const u64 KB = 1024;

    char status[5000] = "ENGINE MEMORY USAGE:\n";
    u64 offset = strlen(status);

    for (u32 i = 0; i < MEM_MAX_TAGS; ++i)
    {
        char unit[3] = "XB";
        float amount = 1.0f;
        
        if ( stats.mem_by_tag[i] >= GB)
        {
            unit[0] = 'G';
            amount = stats.mem_by_tag[i] / (float)GB;
        }
        else if ( stats.mem_by_tag[i] >= GB)
        {
            unit[0] = 'M';
            amount = stats.mem_by_tag[i] / (float)MB;
        }
        else if ( stats.mem_by_tag[i] >= KB)
        {
            unit[0] = 'M';
            amount = stats.mem_by_tag[i] / (float)KB;
        }
        else
        {
            unit[0] = 'B';
            unit[1] = 0;
            amount = (float)stats.mem_by_tag[i];
        }

        offset += snprintf(status + offset, 5000, "\t%s: %.2f%s\n", mem_tag_headers[i], amount, unit);
    }

    // Bump stack char array into heap
    // Bit of performance hit, but this function is a debug function
    char *ret_value = strdup(status);
    return ret_value;
}

void *eri_malloc(u64 size, enum mem_tag tag)
{
    if (tag == MEM_UNKNOWN)
    {
        ERI_LOG_WARNING("MALLOC called with unknown tag, update the allocation");
    }

    // TODO: Implement alignment (?)
    void* memory = platform_malloc(size, FALSE);
    platform_memzero(memory, size);

    stats.total_allocated += size;
    stats.mem_by_tag[tag] += size;

    return memory;
}

void  eri_free(void *memory, u64 size, enum mem_tag tag)
{
    if (tag == MEM_UNKNOWN)
    {
        ERI_LOG_WARNING("FREE called with unknown tag, update the allocation");
    }

    platform_free(memory, FALSE);

    stats.total_allocated -= size;
    stats.mem_by_tag[tag] -= size;
}

void *eri_memset(void *memory, i32 value, u64 size)
{
    return platform_memset(memory, value, size);
}

void *eri_memcpy(void *destination, const void *source, u64 size)
{
    return platform_memcpy(destination, source, size);
}

void *eri_memzero(void *memory, u64 size)
{
    return platform_memzero(memory, size);
}
