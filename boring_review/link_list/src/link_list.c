
#include <stdio.h>
#include <stdlib.h>

#include "link_list.h"

#ifndef LINUX_LIST

typedef struct link_node
{
    void *data;
    struct link_node *next;
}link_node_t;

#endif  /* LINUX_LIST */

struct link_list
{
    link_node_t head;
    int size;
};

link_list_t *init_link_list(void)
{
    link_list_t *p_list = (link_list_t*)calloc(1, sizeof(link_list_t));

    return p_list;
}

#ifdef LINUX_LIST
/* start at index 1  
 * return:  succ 0      fial -1 */
int list_pos_insert(link_list_t *p_list, int pos, link_node_t *p_new_node)
{
    int result = -1;

    if (NULL == p_list || pos <= 0) {
        return result; 
    }
    if (pos > p_list->size) {
        pos = p_list->size + 1;
    }
    link_node_t *p_node = &(p_list->head);

    for (int i = 1; i < pos; i++) {
        p_node = p_node->next;
    }
    p_new_node->next = p_node->next;
    p_node->next = p_new_node;

    p_list->size++;
    
    result = 0;
    return result;
}

int list_push_front(link_list_t *p_list, link_node_t *p_new_node)
{
    return list_pos_insert(p_list, 1, p_new_node);
}

int list_push_back(link_list_t *p_list, link_node_t *p_new_node)
{
    return list_pos_insert(p_list, p_list->size + 1, p_new_node);
}
#else /* LINUX_LIST */

int list_pos_insert(link_list_t *p_list, int pos, void *data)
{
    int result = -1;

    if (NULL == p_list || pos <= 0) {
        return result; 
    }
    if (pos > p_list->size) {
        pos = p_list->size + 1;
    }
    link_node_t *p_node = &(p_list->head);

    for (int i = 1; i < pos; i++) {
        p_node = p_node->next;
    }
    link_node_t *p_new_node = (link_node_t*)calloc(1, sizeof(link_node_t));

    if (NULL == p_new_node) {
        return result; 
    }
    p_new_node->data = data;
    
    p_new_node->next = p_node->next;
    p_node->next = p_new_node;

    p_list->size++;
    
    result = 0;
    return result;
}

int list_push_front(link_list_t *p_list, void *data)
{
    return list_pos_insert(p_list, 1, data);
}

int list_push_back(link_list_t *p_list, void *data)
{
    return list_pos_insert(p_list, p_list->size + 1, data);
}
#endif  /* LINUX_LIST */

int list_pos_remove(link_list_t *p_list, int pos)
{
    int result = -1;

    if (NULL == p_list || 0 == p_list->size
        || pos <= 0 || pos > p_list->size) {
        return result;
    }
    link_node_t *p_node = &(p_list->head);

    for (int i = 1; i < pos; i++) {
        p_node = p_node->next;
    }
    link_node_t *p_del = p_node->next;

    p_node->next = p_del->next;
#ifndef LINUX_LIST
    free(p_del);
#endif
    p_list->size--;

    result = 0;
    return result;
}

int list_pop_front(link_list_t *p_list)
{
    return list_pos_remove(p_list, 1);
}

int list_pop_back(link_list_t *p_list)
{
    return list_pos_remove(p_list, p_list->size);
}

void *get_pos_value(link_list_t *p_list, int pos)
{
    if (NULL == p_list || pos <= 0 || pos > p_list->size) {
        return NULL;
    }
    link_node_t *p_node = &(p_list->head);

    for (int i = 0; i < pos; i++) {
        p_node = p_node->next;
    }
#ifdef LINUX_LIST
    return p_node;
#else
    return p_node->data;
#endif
}

int get_list_size(link_list_t *p_list)
{
    if (NULL == p_list) {
        return -1;
    }
    return p_list->size;
}

void traversal_list(link_list_t *p_list, traversal_cb cb, void *cb_param)
{
    if (NULL == p_list || NULL == cb) {
        return;
    }
    link_node_t *p_node = p_list->head.next;

    while (NULL != p_node)
    {
#ifdef LINUX_LIST
        cb(p_node, cb_param);
#else
        cb(p_node->data, cb_param);
#endif
        p_node = p_node->next;
    }
}

void destroy_link_list(link_list_t *p_list)
{
    if (NULL == p_list) {
        return;
    }
#ifndef LINUX_LIST
    link_node_t *p_node = p_list->head.next;
    link_node_t *p_next = NULL;

    while (NULL != p_node)
    {
        p_next = p_node->next;
        free(p_node);
        
        p_node = p_next;
    }
#endif
    free(p_list);
    p_list = NULL;
}

