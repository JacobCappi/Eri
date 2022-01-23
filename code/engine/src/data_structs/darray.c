#include "core/logger.h"
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
    u64 *full_array = (u64 *)darray - SETTINGS_SIZE;
    u64 settings_size = SETTINGS_SIZE * sizeof(u64);
    u64 full_size = settings_size + ( full_array[DARRAY_CAPACITY] * full_array[DARRAY_STRIDE] );
    eri_free(full_array, full_size, MEM_DARRAY);
}

u64 _darray_get_setting(void *darray, enum darray_settings setting)
{
    u64 *full_array = (u64 *)darray - SETTINGS_SIZE;
    return full_array[setting];
}

void _darray_set_setting(void *darray, enum darray_settings setting, u64 value)
{
    u64 *full_array = (u64 *)darray - SETTINGS_SIZE;
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

void *_darray_insert(void *darray, u64 index, const void* value)
{
    u64 size = darray_get_size(darray);
    u64 stride = darray_get_stride(darray);
    u64 capacity = darray_get_capacity(darray);

    if (index >= size)
    {
        ERI_LOG_ERROR("Index not in bounds, INDEX: %d SIZE: %d", index, size);
        return darray;
    }

    if (size >= capacity)
    {
        darray = _darray_auto_resize(darray);
    }

    u64 starting_address = (u64)darray;

    if (index != (size-1))
    {
        u64 destination = starting_address + (index + 1 * stride);
        u64 source = starting_address + (index * stride);
        eri_memcpy( (void *)destination, (void *)source, stride * (size - index));
    }
    u64 index_location = starting_address + (index * stride);
    eri_memcpy((void *)index_location, value, stride);

    _darray_set_setting(darray, DARRAY_SIZE, size + 1);
    return darray;
}

void *_darray_remove(void *darray, u64 index, void *removed_value)
{
    u64 size = darray_get_size(darray);
    u64 stride = darray_get_stride(darray);
    u64 capacity = darray_get_capacity(darray);

    if (index >= size)
    {
        ERI_LOG_ERROR("Index not in bounds, INDEX: %d SIZE: %d", index, size);
        return darray;
    }

    u64 starting_address = (u64)darray;
    eri_memcpy(removed_value, (void *)(starting_address + (index * stride)), stride);

    if (index != (size-1))
    {
        u64 destination = starting_address + (index * stride);
        u64 source = starting_address + (index + 1 * stride);
        eri_memcpy( (void *)destination, (void *)source, stride * (size - index));
    }

    _darray_set_setting(darray, DARRAY_SIZE, size - 1);
    return darray;
}

void *_darray_push(void *darray, const void* value)
{
    u64 size = darray_get_size(darray);
    u64 stride = darray_get_stride(darray);
    u64 capacity = darray_get_capacity(darray);
    
    if (size >= capacity)
    {
        darray = _darray_auto_resize(darray);
    }

    u64 end_address = (u64)darray;
    end_address += (size*stride);

    eri_memcpy((void *)end_address, value, stride);

    _darray_set_setting(darray, DARRAY_SIZE, size + 1);
    return darray;
}

void *_darray_pop(void *darray, void *popped_value)
{
    u64 size = darray_get_size(darray);
    u64 stride = darray_get_stride(darray);

    u64 end_address = (u64)darray;
    end_address += ( (size-1) *stride);

    eri_memcpy(popped_value, (void *)end_address, stride);
    _darray_set_setting(darray, DARRAY_SIZE, size - 1);
    return darray;
}