#include "darray.h"

#include <stdlib.h>
#include <string.h>

#include "logger.h" //TODO: temp

#define DEFAULT_CAPACITY 2

void* darray_resize(void* array);
u64 darray_get_info(void* array, i32 info_type);
void darray_set_info(void* array, i32 info_type, u64 value); 

void* darray_create(u64 stride) {
    u64 info_size = sizeof(u64)*D_NUM_INFOS;
    u64 array_size = stride*DEFAULT_CAPACITY;
    u64* array = malloc(info_size + array_size);
    memset(array, 0, (info_size + array_size));
    array[D_LENGTH] = 0;
    array[D_STRIDE] = stride;
    array[D_CAPACITY] = DEFAULT_CAPACITY;
    return (void*)(array+D_NUM_INFOS);
}

void* _darray_add(void* array, const void* item) {
    //get info
    u64 length = darray_get_info(array, D_LENGTH);          
    u64 stride = darray_get_info(array, D_STRIDE);          
    u64 capacity = darray_get_info(array, D_CAPACITY);          

    //check if resize needed
    if(length >= capacity) {
        array = darray_resize(array);
    }

    //go to end of array
    char* addr = (char*)array;
    addr += (length*stride);
    memcpy((void*)addr, item, stride);
    darray_set_info(array, D_LENGTH, length+1);
    return array;
}

void darray_destroy(void* array) {
    u64* info = (u64*)array;
    info = info - D_NUM_INFOS;
    free(info);
}

void* darray_resize(void* array) {
    //get info
    u64 length = darray_get_info(array, D_LENGTH);
    u64 stride = darray_get_info(array, D_STRIDE);
    u64 capacity = darray_get_info(array, D_CAPACITY);

    //allocate and set info
    u64 info_size = sizeof(u64)*D_NUM_INFOS;
    u64 array_size = stride*capacity*2; //double capacity
    u64* new_array = malloc(info_size + array_size);
    memset(new_array, 0, (info_size + array_size));
    new_array[D_LENGTH] = length;
    new_array[D_STRIDE] = stride;
    new_array[D_CAPACITY] = capacity*2;

    //copy items from old array
    u64 old_array_data_size = stride*length; 
    new_array = new_array + D_NUM_INFOS;
    memcpy(new_array, array, old_array_data_size);

    return new_array;
}

u64 darray_get_info(void* array, i32 info_type) {
    u64* info = (u64*)array;
    info = info - D_NUM_INFOS;
    return info[info_type];
}

void darray_set_info(void* array, i32 info_type, u64 value) {
    u64* info = (u64*)array;
    info = info - D_NUM_INFOS;
    info[info_type] = value;
}
