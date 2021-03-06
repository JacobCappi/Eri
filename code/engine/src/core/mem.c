#include "core/mem.h"

#include "core/logger.h"
#include "platform/platform.h"

// TODO: Custom String lib (?)
#include "string.h"
#include "stdio.h"

struct mem_stats
{
    u64 total_allocated;
    u64 mem_by_tag[MAX_MEM_TAGS];
};

// TODO: more robust solution
// Should be one to one with the enum
static const char* mem_tag_headers[MAX_MEM_TAGS] = 
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

static b8 is_init = FALSE;
static struct mem_stats stats;

b8 init_memory(void)
{
    ERI_LOG_INFO("Initializing Subsystem: [ Memory ]");

    if (is_init == TRUE)
    {
        ERI_LOG_WARNING("Memory is already initialized");
        return FALSE;
    }

    eri_memzero(&stats, sizeof(struct mem_stats));
    is_init = TRUE;

    return TRUE;
}

void shutdown_memory(void)
{
    // TODO: Assign shutdown if I ever have something to cleanup
    ERI_LOG_INFO("Shutting Down Subsystem: [ Memory ]");
    return;
}

// Debug function to output all malloc totals by tag
char* mem_get_status(void)
{
    ERI_LOG_INFO("Debug function \"mem_get_status\" called");
    const u64 GB = 1024 * 1024 * 1024;
    const u64 MB = 1024 * 1024;
    const u64 KB = 1024;

    char status[5000] = "ENGINE MEMORY USAGE:\n";
    u64 offset = strlen(status);

    for (u32 i = 0; i < MAX_MEM_TAGS; ++i)
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

    // TODO: Fix this with custom stuff....
#if ERI_PLATFORM_WINDOWS
    char *ret_value = _strdup(status);
#else
    char *ret_value = strdup(status);
#endif

    return ret_value;
}

void *eri_malloc(u64 size, enum mem_tag tag)
{
    ERI_LOG_DEBUG("Malloc called for %ld bytes", size);
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

void eri_free(void *memory, u64 size, enum mem_tag tag)
{
    ERI_LOG_DEBUG("Free called for %ld bytes", size);
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
