
#include <stdio.h>

#include "link_list.h"

typedef struct person
{
#ifdef LINUX_LIST
    link_node_t node;
#endif
    const char *name;
    int age;
}person_t;

void callback(void *p_data, void *cb_param)
{
    if (NULL == p_data && NULL == cb_param) {
        return;
    }
    person_t *ap = (person_t*)p_data;

    printf("%s name: %s    age: %d\n", (char*)cb_param, ap->name, ap->age);
}

void test_link_list(int argc, char *argv[])
{
    link_list_t *p_list = init_link_list();

#ifdef LINUX_LIST
    link_node_t node = { NULL };
    person_t p1 = { node, "liban", 22 };
    person_t p2 = { node, "snail", 23 };
    person_t p3 = { node, "fake", 17 };
    person_t p4 = { node, "abc", 15 };
    person_t p5 = { node, "cbd", 27 };
    person_t p6 = { node, "111", 111 };

    list_push_front(p_list, (link_node_t*)&p1);
    list_push_front(p_list, (link_node_t*)&p2);
    
    list_push_back(p_list, (link_node_t*)&p3);
    list_push_back(p_list, (link_node_t*)&p4);
    list_push_back(p_list, (link_node_t*)&p5);
    
    list_pos_insert(p_list, 3, (link_node_t*)&p6);
#else   /* LINUX_LIST */
    person_t p1 = { "liban", 22 };
    person_t p2 = { "snail", 23 };
    person_t p3 = { "fake", 17 };
    person_t p4 = { "abc", 15 };
    person_t p5 = { "cbd", 27 };
    person_t p6 = { "111", 111 };

    list_push_front(p_list, &p1);
    list_push_front(p_list, &p2);
    
    list_push_back(p_list, &p3);
    list_push_back(p_list, &p4);
    list_push_back(p_list, &p5);
    
    list_pos_insert(p_list, 3, &p6);

#endif  /* LINUX_LIST */

    traversal_list(p_list, callback, "----");

    person_t *pp = (person_t*)get_pos_value(p_list, 1);

    if (NULL != pp) {
        printf("name: %s    age: %d\n", pp->name, pp->age);
    }
    // printf("list size: %d\n", get_list_size(p_list));

    list_pop_front(p_list);
    
    list_pop_back(p_list);
    
    list_ptr_remove(p_list, &p3);
    
    traversal_list(p_list, callback, "====");

    destroy_link_list(p_list);
    p_list = NULL;
}

int main(int argc, char *argv[])
{
    test_link_list(argc, argv);

    return 0;
}

