
#ifndef BASEINFO_COLLECT_HH
#define BASEINFO_COLLECT_HH

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct collect_handle collect_handle_t;

typedef enum
{
    BASEINFO_TYPE_INVALID   = 0,

    BASEINFO_TYPE_CSTRING   = 1 << 0,
    BASEINFO_TYPE_INTEGER   = 1 << 1,
    BASEINFO_TYPE_DOUBLE    = 1 << 2,
    BASEINFO_TYPE_BINARY    = 1 << 3,

    BASEINFO_TYPE_BOUNDARY  = 1 << 31,

}baseinfo_type_t;

typedef struct
{
    const char *name;

    const uint8_t *p_data;
    uint32_t data_len;

    baseinfo_type_t type;

}collect_item_t;

collect_handle_t *create_collect_handle(void);

/* Gets static and dynamic infos, usually called only once.
 * The caller doesn't need to free memory.
 *
 * param p_item_arr
 *      Array of collect_item_t *, output parameter
 *      The caller needs to input the address of the first-level pointer
 *
 * param p_count
 *      Count of the collect_item_t * array, output parameter
 */
bool collect_infos(collect_handle_t *p_collect,
                   collect_item_t **p_item_arr, uint32_t *p_count);

/* Updating the dynamic infos in the collected infos.
 * This operation is multi-thread safe.
 *
 * This function will change the contents of the collect_item_t * array.
 */
bool update_collect_infos(collect_handle_t *p_collect);

/* When p_collect is destroyed, collect_item_t * array will not exist. */
void destroy_collect_handle(collect_handle_t *p_collect);

#ifdef __cplusplus
}
#endif

#endif /* BASEINFO_COLLECT_HH */
