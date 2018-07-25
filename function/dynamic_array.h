
#ifndef DYNAMIC_ARRAY_HHH
#define DYNAMIC_ARRAY_HHH

#include <stdint.h>

typedef struct dynamic_array dynamic_array_t;

typedef void(*traverse_cb)(void *p_item, void *cb_param);

/* If init_capacity is 0 the default capacity will be used 
 * return:  succ p_array    fail NULL */
dynamic_array_t* init_dynamic_array(uint32_t init_capacity);

 /* return:  succ 0    fail -1 */
int darray_index_insert(dynamic_array_t *p_array, uint32_t index, void *p_item);

int darray_push_back(dynamic_array_t *p_array, void *p_item);

int darray_push_front(dynamic_array_t *p_array, void *p_item);

int darray_index_remove(dynamic_array_t *p_array, uint32_t index);

int darray_pop_back(dynamic_array_t *p_array);

int darray_pop_front(dynamic_array_t *p_array);

/* return:  succ p_item fail NULL */
void* get_index_item(dynamic_array_t *p_array, uint32_t index);

/* return:  succ size   fail -1 */
int get_darray_size(dynamic_array_t *p_array);

void traverse_darray(dynamic_array_t *p_array, traverse_cb cb, void *cb_param);

void destroy_dynamic_array(dynamic_array_t *p_array);


#endif /* DYNAMIC_ARRAY_HHH */
