#include "punk_queue.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct punk_queue
{
    void** priv_array;
    uint32_t head;
    uint32_t tail;
    uint32_t capacity;
    uint32_t size;
};

/* Specify queue capacity initialization */
punk_queue_t* create_queue(uint32_t capacity)
{
    if (0 == capacity) {
        return NULL;
    }
    capacity++; // for next index

    punk_queue_t *result = (punk_queue_t*)calloc(1, sizeof(punk_queue_t));
    result->capacity = capacity;

    result->priv_array = (void**)calloc(capacity, sizeof(void*));

    if (NULL == result->priv_array) {
        free (result);
        result = NULL;
    }
    return result;
}

bool queue_is_full(punk_queue_t *p_queue)
{
    return (p_queue->tail+ 1) % p_queue->capacity == p_queue->head;
}

bool enqueue(punk_queue_t *p_queue, void* data)
{
    bool result = false;
    if (NULL != p_queue && !queue_is_full(p_queue))
    {
        p_queue->priv_array[p_queue->tail] = data;
        // next index
        p_queue->tail = (p_queue->tail+ 1) % p_queue->capacity;
        p_queue->size++;
        result = true;
    }
    return result;
}

bool queue_is_empty(punk_queue_t *p_queue)
{
    return p_queue->head == p_queue->tail;
}

void* dequeue(punk_queue_t *p_queue)
{
    void *result = NULL;
    if (NULL != p_queue && !queue_is_empty(p_queue))
    {
        result = p_queue->priv_array[p_queue->head];
        p_queue->head = (p_queue->head + 1) % p_queue->capacity;
        p_queue->size--;
    }
    return result;
}

uint32_t queue_size(punk_queue_t *p_queue)
{
    return (NULL == p_queue) ? 0 : p_queue->size;
}

void destroy_queue(punk_queue_t *p_queue)
{
    if (NULL != p_queue)
    {
        if (p_queue->priv_array)
            free(p_queue->priv_array);

        free(p_queue);
    }
}

