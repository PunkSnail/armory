#ifndef PUNK_QUEUE_HHH
#define PUNK_QUEUE_HHH

#include <stdint.h>

// Definition of queue_item_t could be changed to any type 
// in source code, before compiling code.
typedef void* queue_item_t;

typedef struct punk_queue punk_queue_t;

typedef void(*queue_traverse_t)(void *p_item, void *cb_param);

/* return:  succ p_queue    fail NULL */
punk_queue_t* create_punk_queue(uint32_t capacity);

/* return:  succ 0    fail -1 */
int punk_enqueue(punk_queue_t *p_queue, queue_item_t data);

/* return:  succ p_item    fail NULL */
queue_item_t* punk_dequeue(punk_queue_t *p_queue);

uint32_t get_queue_size(punk_queue_t *p_queue);

void destroy_punk_queue(punk_queue_t *p_queue);

#endif /* PUNK_QUEUE_HHH */

