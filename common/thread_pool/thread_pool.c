
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

#include "thread_pool.h"

typedef struct
{
    pthread_t id;
    void *p_params;
    bool is_running;

}thread_context_t;

struct thread_pool
{
    uint8_t     curr_thread_index;

    thread_cb_t thread_cb;
    push_cb_t   push_cb;

    thread_context_t *p_contexts;
    uint8_t     thread_count;
};

static void init_thread_contexts(thread_context_t *p_contexts, 
                                void **pp_thread_params, uint8_t thread_count)
{
    for (uint8_t i = 0; i < thread_count; i++)
    {
        p_contexts[i].p_params  = pp_thread_params[i];
        p_contexts[i].is_running = false;
    }
}

static bool check_params(const pool_params_t *p_parmas)
{
    bool result = false;

    if (NULL != p_parmas 
        && NULL != p_parmas->thread_cb 
        && NULL != p_parmas->push_cb
        && NULL != p_parmas->push_cb 
        && 0 < p_parmas->thread_count)
    {
        result = true;
    }
    return result;
}

thread_pool_t *create_thread_pool(const pool_params_t *p_parmas)
{
    thread_pool_t *result = NULL;

    if (false == check_params(p_parmas)) {
        return result;
    }
    thread_pool_t *p_pool = (thread_pool_t*)calloc(1, sizeof(thread_pool_t));
    
    thread_context_t *p_contexts = 
        (thread_context_t*)calloc(p_parmas->thread_count, sizeof(thread_context_t));

    if (NULL != p_contexts && NULL != p_pool)
    {
        p_pool->thread_cb       = p_parmas->thread_cb;
        p_pool->push_cb         = p_parmas->push_cb;
        p_pool->thread_count    = p_parmas->thread_count;

        init_thread_contexts(p_contexts, 
                p_parmas->pp_thread_params, p_parmas->thread_count);

        p_pool->p_contexts = p_contexts;

        result = p_pool;
    }
    return result;
}

static int push_to_new_thread(thread_pool_t *p_pool, 
                thread_context_t *p_context, void *p_data, uint32_t data_len)
{
    int result = pthread_create(&p_context->id, NULL, 
                                p_pool->thread_cb, p_context->p_params);
    if (0 == result) {
        result = p_pool->push_cb(p_context->p_params, p_data, data_len);
        p_context->is_running = true;
    }
    return result;
}

static uint8_t get_thread_index(thread_pool_t *p_pool)
{
    uint8_t result = p_pool->curr_thread_index;

    p_pool->curr_thread_index = (uint8_t)(result + 1) % p_pool->thread_count;

    return result;
}

int thread_pool_push_data(thread_pool_t *p_pool, 
                          void *p_data, uint32_t data_len)
{
    int result = -1;

    if (NULL == p_pool) {
        return result;
    }
    uint8_t thread_index = get_thread_index(p_pool);

    if (thread_index >= p_pool->thread_count) {
        return result;
    }
    thread_context_t *p_context = p_pool->p_contexts + thread_index;
    
    if (true == p_context->is_running)
    {
        result = p_pool->push_cb(p_context->p_params, p_data, data_len);
    }
    else {
        result = push_to_new_thread(p_pool, p_context, p_data, data_len);
    }
    return result;
}

static void cancel_all_thread(thread_pool_t *p_pool)
{
    thread_context_t *p_contexts = p_pool->p_contexts;

    for (uint8_t i = 0; i < p_pool->thread_count; i++)
    {
        if (true == p_contexts[i].is_running)
        {
            pthread_cancel(p_contexts[i].id);
            p_contexts[i].is_running = false;
        }
    }
}

void destroy_thread_pool(thread_pool_t *p_pool)
{
    if (NULL == p_pool)
        return;

    cancel_all_thread(p_pool);

    if (NULL != p_pool->p_contexts) {
        free(p_pool->p_contexts);
    }
    free(p_pool);
    p_pool = NULL;
}

