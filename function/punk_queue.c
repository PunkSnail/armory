#include "punk_queue.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct punk_queue
{
	queue_item_t* priv_array;
	uint32_t head;
	uint32_t tail;
	uint32_t capacity;
	uint32_t size;
};

static uint32_t next_index(uint32_t current_index, uint32_t modulo)
{
    return (current_index + 1) % modulo;
}

static bool judge_queue_full(punk_queue_t *p_queue)
{
    bool result = false;
    
    if (next_index(p_queue->tail, p_queue->capacity) == p_queue->head)
    {
        result = true;
    }
    return result;
}

punk_queue_t* create_punk_queue(uint32_t capacity)
{
    if (0 == capacity) {
        return NULL;
    }
    capacity++;

    punk_queue_t *result = (punk_queue_t*)calloc(1, sizeof(punk_queue_t));
    result->capacity = capacity;
    
    result->priv_array = (queue_item_t*)calloc(capacity, sizeof(queue_item_t));
    
    if (NULL == result->priv_array) {
#ifdef _DEbug
        perror("create_punk_queue calloc error");
#endif
        free (result);
        result = NULL;
    }
    return result;
}

int punk_enqueue(punk_queue_t *p_queue, queue_item_t data)
{
    int result = -1;
    if (NULL != p_queue && (false == judge_queue_full(p_queue)))
    {
        p_queue->priv_array[p_queue->tail] = data;
        p_queue->tail = next_index(p_queue->tail, p_queue->capacity);
        p_queue->size++;
        result = 0;
    }
    return result;
}

static bool judge_queue_empty(punk_queue_t *p_queue)
{
    bool result = false;
    if (p_queue->head == p_queue->tail)
    {
        result = true;
    }
    return result;
}

queue_item_t* punk_dequeue(punk_queue_t *p_queue)
{
    queue_item_t *result = NULL;
    if (NULL != p_queue && (false == judge_queue_empty(p_queue)))
    {
        result = p_queue->priv_array[p_queue->head];
        p_queue->head = next_index(p_queue->head, p_queue->capacity);
        p_queue->size--;
    }
    return result;
}

uint32_t get_queue_size(punk_queue_t *p_queue)
{
    if (NULL == p_queue)
        return 0;

    return p_queue->size;
}

/* void traverse_queue(punk_queue_t *p_queue, queue_traverse_t cb, void *cb_param) */
/* { */
/*     if (NULL == p_queue || NULL == cb) { */
/*         return; */
/*     } */
/*     for (uint32_t i = 0; i < p_queue->size; i++) { */
/*         cb(p_queue->priv_array[i], cb_param); */
/*     } */
/* } */

void destroy_punk_queue(punk_queue_t *p_queue)
{
    if (NULL != p_queue)
    {
        if (p_queue->priv_array)
            free(p_queue->priv_array);

        free(p_queue);
    }
}

