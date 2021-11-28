#include "core/mem.h"
#include "data_structs/darray.h"


void *_darray_create(u64 size, u64 stride)
{
    u64 settings_size = SETTINGS_SIZE * sizeof(u64);
    u64 data_size = size * stride;

    // u64* to properly store settings
    u64 *full_array = eri_malloc(settings_size + data_size, MEM_DARRAY);
    full_array[DARRAY_CAPACITY] = size;
    full_array[DARRAY_SIZE] = 0;
    full_array[DARRAY_STRIDE] = stride;

    // return void* with offset to hide settings
    return (void *)(full_array + SETTINGS_SIZE);
}

void _darray_destroy(void *darray)
{
    u64 *full_array = darray - SETTINGS_SIZE;
    u64 settings_size = SETTINGS_SIZE * sizeof(u64);
    u64 full_size = settings_size + ( full_array[DARRAY_CAPACITY] * full_array[DARRAY_STRIDE] );
    eri_free(full_array, full_size, MEM_DARRAY);
}

u64 _darray_get_setting(void *darray, darray_settings setting)
{
    u64 *full_array = darray - SETTINGS_SIZE;
    return full_array[setting];
}

void _darray_set_setting(void *darray, darray_settings setting, u64 value)
{
    u64 *full_array = darray - SETTINGS_SIZE;
    full_array[setting] = value;
}

// Will just double array size for minimal performance use
void *_darray_auto_resize(void *darray)
{
    u64 size = darray_get_size(darray);
    u64 stride = darray_get_stride(darray);
    u64 capacity = darray_get_capacity(darray);

    void *new_array = _darray_create( (capacity * DARRAY_AUTO_RESIZE_FACTOR), stride);

    // Size is used here, as capacity would be redudant
    eri_memcpy(new_array, darray, size * stride);

    _darray_set_setting(new_array, DARRAY_SIZE, size);
    _darray_destroy(darray);
    return new_array;
}

    // 43
void *_darray_insert(void *darray, u64 index, const void* value);

void *_darray_remove(void *darray, u64 index, void *removed_value);

void *_darray_push(void *darray, const void* value)
{
    u64 size = darray_get_size(darray);
    u64 stride = darray_get_stride(darray);
    u64 capacity = darray_get_capacity(darray);
    
    if (size >= capacity)
    {
        darray = _darray_auto_resize(darray);
    }
    // TODO: this probably doesn't work b/c darray gets moved, and I don't move it back.
    // Need to fix
    u64 *end_address = darray + (size - 1);
    eri_memcpy(end_address, value, stride);

    _darray_set_setting(darray, DARRAY_SIZE, size + 1);
    return darray;
}

void *_darray_pop(void *darray, void *popped_value)
{
    u64 size = darray_get_size(darray);
    u64 stride = darray_get_stride(darray);

    u64 *end_address = darray + (size - 1);
    eri_memcpy(popped_value, end_address, stride);
    _darray_set_setting(darray, DARRAY_SIZE, size - 1);
}