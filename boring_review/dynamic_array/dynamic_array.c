
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dynamic_array.h"

#define DEFAULT_CAPACITY    10

#ifdef _DEBUG

#define debug_print(...) do{ printf(__VA_ARGS__); }while(0)

#else

#define debug_print(...) do{ }while(0)

#endif /* _DEBUG */

struct dynamic_array
{
    void **pp_addr;     // member address
    uint32_t capacity;
    uint32_t size;
};

dynamic_array_t* init_dynamic_array(uint32_t init_capacity)
{
    if (0 == init_capacity) {
        init_capacity = DEFAULT_CAPACITY;
    }
    dynamic_array_t *p_array = (dynamic_array_t*)calloc(1, sizeof(dynamic_array_t));

    if (p_array) {
        p_array->capacity   = init_capacity;
        p_array->size       = 0;
        p_array->pp_addr    = (void**)calloc(p_array->capacity, sizeof(void*));

        if (NULL == p_array->pp_addr) {
#ifdef _DEBUG
            perror("init_dynamic_array calloc error");
#endif
            free(p_array);
            p_array = NULL;
        }
    }
    return p_array;
}

int darray_index_insert(dynamic_array_t *p_array, uint32_t index, void *p_item)
{
    int result = -1;
    
    if (NULL == p_array || index > p_array->capacity)
        return result;

    if (p_array->size == p_array->capacity)
    {
        uint32_t new_capacity = p_array->capacity * 2;
        
        void **new_space = (void**)calloc(new_capacity, sizeof(void*));

        if (NULL == new_space) {
#ifdef _DEBUG
            perror("darray_insert calloc error");
#endif
            return result;
        }
        memcpy(new_space, p_array->pp_addr, sizeof(void*) * p_array->capacity);

        free(p_array->pp_addr);
        
        p_array->pp_addr = new_space;
        p_array->capacity = new_capacity;
    }
    /* move position, explicit cast "int32_t" */
    for (int32_t i = (int32_t)p_array->size - 1; i >= (int32_t)index; i--) {
        p_array->pp_addr[i + 1] = p_array->pp_addr[i];
    }
    p_array->pp_addr[index] = p_item;
    p_array->size++;
    
    result = 0;
    return result;
}

int darray_push_back(dynamic_array_t *p_array, void *p_item)
{
    return darray_index_insert(p_array, p_array->size, p_item);
}

int darray_push_front(dynamic_array_t *p_array, void *p_item)
{
    return darray_index_insert(p_array, 0, p_item);
}

int darray_index_remove(dynamic_array_t *p_array, uint32_t index)
{
    int result = -1;
    
    if (NULL == p_array || index >= p_array->size) {
        return result;
    }
    for (uint32_t i = index; i < p_array->size - 1; i++) {
        p_array->pp_addr[i] = p_array->pp_addr[i + 1];
    }
    p_array->size--;
    
    result = 0;
    return result;
}

int darray_pop_back(dynamic_array_t *p_array)
{
    if (NULL == p_array || 0 == p_array->size) {
        return -1;
    }
    return darray_index_remove(p_array, p_array->size - 1);
}

int darray_pop_front(dynamic_array_t *p_array)
{
    if (NULL == p_array || 0 == p_array->size) {
        return -1;
    }
    return darray_index_remove(p_array, 0);
}

void* get_index_item(dynamic_array_t *p_array, uint32_t index)
{
    if (NULL == p_array || index >= p_array->size) {
        return NULL;
    }
    return p_array->pp_addr[index];
}

int get_darray_size(dynamic_array_t *p_array)
{
    if (NULL == p_array)
        return -1;

    return (int)p_array->size;
}

void traverse_darray(dynamic_array_t *p_array, traverse_cb cb, void *cb_param)
{
    if (NULL == p_array || NULL == cb) {
        return;
    }
    for (uint32_t i = 0; i < p_array->size; i++) {
        cb(p_array->pp_addr[i], cb_param);
    }
}

void destroy_dynamic_array(dynamic_array_t *p_array)
{
    if (p_array) {
        if (p_array->pp_addr) {
            free(p_array->pp_addr);
            p_array->pp_addr = NULL;
        }
        free(p_array);
        p_array = NULL;
    }
}
