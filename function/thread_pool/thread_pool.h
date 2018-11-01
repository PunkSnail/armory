
#ifndef THREAD_POOL_HH
#define THREAD_POOL_HH

#include <stdint.h>

typedef struct thread_pool thread_pool_t;

typedef void *(*thread_cb_t) (void *p_thread_params);

/* return:  succ 0   fail -1 */
typedef int (*push_cb_t) 
    (void *p_thread_params, void *p_data, uint32_t data_len);

typedef struct
{
    thread_cb_t thread_cb;
    void        **pp_thread_params;
    
    push_cb_t   push_cb;

    uint8_t     thread_count;
}pool_params_t;

/* return:  succ p_pool   fail NULL */
thread_pool_t *create_thread_pool(const pool_params_t *p_parmas);

/* return:  succ 0   fail -1 */
int thread_pool_push_data(thread_pool_t *p_pool, 
                          void *p_data, uint32_t data_len);

void destroy_thread_pool(thread_pool_t *p_pool);


#endif  /* THREAD_POOL_HH */
