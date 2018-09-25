
#ifndef LINK_LIST_HH
#define LINK_LIST_HH

#ifdef __cplusplus
extern "C" {
#endif

#ifdef LINUX_LIST

typedef struct link_node
{
    struct link_node *next;
}link_node_t;
#endif

typedef struct link_list link_list_t;

typedef void (traversal_cb)(void *p_data, void *cb_param);

/* return:  succ p_list     fial NULL */
link_list_t *init_link_list(void);

#ifdef LINUX_LIST
/* start at index 1  
 * return:  succ 0      fial -1 */
int list_pos_insert(link_list_t *p_list, int pos, link_node_t *p_new_node);

int list_push_front(link_list_t *p_list, link_node_t *p_new_node);

int list_push_back(link_list_t *p_list, link_node_t *p_new_node);

#else

/* start at index 1  
 * return:  succ 0      fial -1 */
int list_pos_insert(link_list_t *p_list, int pos, void *data);

int list_push_front(link_list_t *p_list, void *data);

int list_push_back(link_list_t *p_list, void *data);

#endif  /* LINUX_LIST */

/* start at index 1 */
int list_pos_remove(link_list_t *p_list, int pos);

int list_pop_front(link_list_t *p_list);

int list_pop_back(link_list_t *p_list);

/* start at index 1  
 * return:  succ p_data     fial NULL */
void *get_pos_value(link_list_t *p_list, int pos);

/* return:  succ 0      fial -1 */
int get_list_size(link_list_t *p_list);

void traversal_list(link_list_t *p_list, traversal_cb cb, void *cb_param);

void destroy_link_list(link_list_t *p_list);

#ifdef __cplusplus
}
#endif

#endif /* LINK_LIST_HH */

