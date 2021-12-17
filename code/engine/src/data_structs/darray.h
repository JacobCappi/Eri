#pragma once

#include "defines.h"

/*
    darray implementation 
    -----
    [0] u64 CAPACITY [How much reserved]
    [1] u64 SIZE     [How much in use]
    [2] u64 STRIDE   [size of the element]
    ----- Offset input index
    [0] void* DATA [really 3]
*/

 enum darray_settings
{
    DARRAY_CAPACITY,
    DARRAY_SIZE,
    DARRAY_STRIDE,

    SETTINGS_SIZE 
};

// ----- Init Functions
ERI_API void *_darray_create(u64 capacity, u64 stride);
ERI_API void _darray_destroy(void *darray);
// ----- END

// ----- 
ERI_API u64 _darray_get_setting(void *darray, enum darray_settings setting);
ERI_API void _darray_set_setting(void *darray, enum darray_settings setting, u64 value);

// Will just double array size for minimal performance use
ERI_API void *_darray_auto_resize(void *darray);

ERI_API void *_darray_insert(void *darray, u64 index, const void* value);
ERI_API void *_darray_remove(void *darray, u64 index, void *removed_value);

ERI_API void *_darray_push(void *darray, const void* value);
ERI_API void *_darray_pop(void *darray, void *popped_value);
// ----- END

#define DARRAY_DEFAULT_CAPACITY 8
#define DARRAY_AUTO_RESIZE_FACTOR 2

#define darray_create_default(type)                        \
    _darray_create(DARRAY_DEFAULT_CAPACITY, sizeof(type))

#define darray_create(type, capacity)                      \
    _darray_create(capacity, sizeof(type))

#define darray_destroy(darray)                             \
    _darray_destroy(darray)

#define darray_insert(darray, index, value)                \
    {                                                      \
        typeof(value) in_value = value;                    \
        darray = _darray_push(darray, index, &in_value);   \
    }                                                      \

#define darray_remove(darray, index, value)                \
    _darray_remove(darray, index, value)

#define darray_push(darray, value)                         \
    {                                                      \
        typeof(value) in_value = value;                    \
        darray = _darray_push(darray, &in_value);          \
    }                                                      \

#define darray_pop(darray, value)                          \
    _darray_pop(darray, value)

#define darray_clear(darray)                               \
    _darray_set_setting(darray, DARRAY_SIZE, 0)

#define darray_get_capacity(darray)                        \
    _darray_get_setting(darray, DARRAY_CAPACITY)

#define darray_get_size(darray)                            \
    _darray_get_setting(darray, DARRAY_SIZE)

#define darray_get_stride(darray)                          \
    _darray_get_setting(darray, DARRAY_STRIDE)

#define darray_set_size(darray, size)                      \
    _darray_set_setting(darray, DARRAY_SIZE, size)
