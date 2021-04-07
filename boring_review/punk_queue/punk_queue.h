#ifndef PUNK_QUEUE_HHH
#define PUNK_QUEUE_HHH

#include <stdint.h>
#include <stdbool.h>

typedef struct punk_queue punk_queue_t;

/* return:  succ p_queue    fail NULL */
punk_queue_t* create_queue(uint32_t capacity);

bool queue_is_empty(punk_queue_t *p_queue);

bool queue_is_full(punk_queue_t *p_queue);

/* when the queue is full, enqueue will fail */
bool enqueue(punk_queue_t *p_queue, void* data);

/* when the queue is empty, dequeue will fail */
void* dequeue(punk_queue_t *p_queue);

uint32_t queue_size(punk_queue_t *p_queue);

void destroy_queue(punk_queue_t *p_queue);

#endif /* PUNK_QUEUE_HHH */

