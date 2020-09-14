#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "baseinfo_collect.h"

void show_collet_item(collect_item_t *p_item)
{
    if (NULL == p_item)
    {
        return;
    }
    printf("%s:\t", p_item->name);

    switch (p_item->type)
    {
    case BASEINFO_TYPE_CSTRING:
        printf("%s\n", (const char*)p_item->p_data);
        break;
    case BASEINFO_TYPE_INTEGER:
        printf("%d\n", *(const int32_t*)p_item->p_data);
        break;
    default:
        printf("unknown data type %u\n", p_item->type);
        break;
    }
}

int main(void)
{
    collect_handle_t *p_collect = create_collect_handle();
    collect_item_t *item_arr = NULL;
    uint32_t item_count = 0;

    if (NULL == p_collect)
    {
        return -1;
    }
    collect_infos(p_collect, &item_arr, &item_count);

    update_collect_infos(p_collect);

    for (uint32_t i = 0; i < item_count; i++)
    {
         show_collet_item(&item_arr[i]);
    }
    destroy_collect_handle(p_collect);
    p_collect = NULL;

    return 0;
}
