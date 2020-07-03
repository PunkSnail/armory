
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <getopt.h> /* getopt_long */

#include "classic_sort.h"

typedef void (*sort_func_t)(int *array, uint32_t len);

typedef struct
{
    const char *name;
    sort_func_t func;
}sort_t;

static const sort_t g_algorithm_arr[] =
{
    {"bubble", bubble_sort},
    {"select", select_sort},
    {"insert", insert_sort},
    {"quick", quick_sort},
    {"shell", shell_sort},
    {"merger", merge_sort},
};

void show_help(void)
{
    printf("Options:\n"
           "-s <size>       Specify the array size\n"
           "-l <limit>      Maximum element limit\n"
           "-a <algorithm>  Specify sort algorithm:");

    for (int i = 0; i < sizeof(g_algorithm_arr)/sizeof(sort_t); i++)
    {
        i ? printf(", ") : printf(" ");

        printf("\"%s\"", g_algorithm_arr[i].name);
    }
    printf("\n-d, --display   Display the array detail\n"
           "-h, --help      Display this information\n");
}

static int *create_test_arr(uint32_t size, int limit)
{
    int *array = (int*)calloc(size, sizeof(int));

    if (NULL == array)
    {
        perror("create_test_arr");
        return array;
    }
    for (uint32_t i = 0; i < size; i++)
    {
        array[i] = rand() % limit;
    }
    return array;
}

static int *shuffling(int *array, uint32_t size)
{
    uint32_t swap_index = 0;

    for (uint32_t i = 0; i < size; i++)
    {
        swap_index = (uint32_t)rand() % size;

        if (i != swap_index) {
            swap_value(array[swap_index], array[i]);
        }
    }
    return array;
}

static void print_array(int *array, uint32_t size)
{
    for (uint32_t i = 0; i < size; i++)
        printf("%d ", array[i]);

    printf("\n");
}

static void
test_sort_func(int *array, uint32_t len, const char *algorithm, bool show)
{
    int i = 0;
    bool match = false;
    sort_func_t sort = NULL;

    for (; i < sizeof(g_algorithm_arr)/sizeof(sort_t); i++)
    {
        if (algorithm && 0 == strcmp(algorithm, g_algorithm_arr[i].name))
        {
            match = true;
            break;
        }
    }
    i = match ? i : 0;
resrot:
    sort = g_algorithm_arr[i].func;
    algorithm = g_algorithm_arr[i].name;

    show ? printf("\nshuffling:\n") : 0;

    shuffling(array, len);

    show ? print_array(array, len) : 0;

    time_t start = clock();
    sort(array, len);
    time_t end = clock();

    printf("%s sort: %f\n", algorithm, (double)(end - start)/CLOCKS_PER_SEC);

    show ? print_array(array, len) : 0;

    if (false == match && i < sizeof(g_algorithm_arr)/sizeof(sort_t) - 1)
    {
         i++;
         goto resrot;
    }
}

int main(int argc, char *argv[])
{
    srand((unsigned int)time(NULL));

    bool show = false;
    uint32_t size = 20;
    int num_limit = 100;
    const char *algorithm = NULL;

    int opt;
    static struct option ops[] = {
        { "size", required_argument, NULL, 's' },
        { "limit", required_argument, NULL, 'l' },
        { "algorithm", required_argument, NULL, 'a' },
        { "display", no_argument, NULL, 'd' },
        { "help", no_argument, NULL, 'h' }
    };

    while ((opt = getopt_long(argc, argv, "s:l:a:dh", ops, NULL)) != -1)
    {
        switch(opt)
        {
        case 's':
            size = (uint32_t)atoi(optarg);
            break;
        case 'l':
            num_limit = atoi(optarg);
            break;
        case 'a':
            algorithm = optarg;
            break;
        case 'd':
            show = true;
            break;
        case 'h':
            show_help();
            return 0;
        default:
            fprintf(stderr, "Try: %s --help\n", argv[0]);
            return -1;
        }
    }
    printf("array size: %u\n", size);

    int *array = create_test_arr(size, num_limit);

    if (NULL == array)
        return -1;

    test_sort_func(array, size, algorithm, show);

    free(array);
    return 0;
}

