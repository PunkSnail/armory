
#include <stdio.h>
#include <stdlib.h>

#include "thread_pool.h"

void *thread_func(void *p_thread_params)
{
    printf("%s start\n", __func__);

    return NULL;
}

int push_cb(void *p_thread_params, void *p_data, uint32_t data_len)
{
    printf("%s start\n", __func__);
    
    return 0;
}

int main(int argc, char *argv[])
{
    uint8_t thread_count = 3;

    char **pp_thread_params = (char**)calloc(thread_count, sizeof(char*));
    pool_params_t parmas = {
        .thread_cb = thread_func,
        .pp_thread_params = (void**)pp_thread_params,
        .push_cb = push_cb,
        .thread_count = thread_count
    };
    thread_pool_t *p_pool = create_thread_pool(&parmas);

    if (NULL == p_pool) {
        printf("create_thread_pool fail\n");
        goto err_end;
    }
    destroy_thread_pool(p_pool);
    p_pool = NULL;

err_end:
    free(pp_thread_params);
    pp_thread_params = NULL;

    printf("...\n");
    return 0;
}
