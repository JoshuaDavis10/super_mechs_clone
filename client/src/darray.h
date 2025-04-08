#pragma once

#include "defines.h"

enum {
    D_LENGTH,
    D_STRIDE,
    D_CAPACITY,
    D_NUM_INFOS 
};

void* darray_create(u64 stride);
void* _darray_add(void* array, const void* item);
void darray_destroy(void* darray);
u64 darray_get_info(void* array, i32 info_type); 
#define darray_add(array, item) \
{                                   \
    __auto_type temp = item; \
    array = _darray_add(array, &temp); \
}
