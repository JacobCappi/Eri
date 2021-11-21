#include "mem.h"

#include "core/logger.h"
#include "platform/platform.h"

struct mem_stats
{
    u64 total_allocated;
    u64 mem_by_tag[MEM_MAX_TAGS];
};

static struct mem_stats stats;


void init_memory(void)
{
    platform_memzero(&stats, sizeof(stats));
}

void shutdown_memory(void);

char* get_mem_status(void);

void  eri_free(void *memory, u64 size, mem_tag tag);
void *eri_malloc(u64 size, mem_tag tag);

void *eri_memset(void *memory, i32 value, u64 size);
void *eri_memcpy(void *source, const void *destination, u64 size);
void *eri_memzero(void *memory, u64 size);
