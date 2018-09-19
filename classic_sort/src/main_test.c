
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>

#include "classic_sort.h"

#define RAND_NUMBER_LIMIT 100000
#define DEFAULT_ARRAY_LEN 20

typedef void (*sort_func_t)(int *array, int len);

int *get_test_array(int size)
{
    int *array = (int*)calloc((size_t)size, sizeof(int));

    if (NULL == array) {
        return array;
    }
    for (int i = 0; i < size; i++) {
        array[i] = rand() % RAND_NUMBER_LIMIT;
    }
    return array;
}

int *shuffling(int *array, int size)
{
    int swap_index = 0;
    
    for (int i = 0; i < size; i++)
    {
        swap_index = rand() % size;
        
        if (i != swap_index) {
            swap_value(array[swap_index], array[i]);
        }
    }
    return array;
}

void free_test_array(int *array)
{
    if (array)
        free(array);
}

static void print_array(int *array, int size)
{
#ifdef _SHOW_ARRAY
    for (int i = 0; i < size; i++)
        printf("%d ", array[i]);

    printf("\n");
#endif
}

void test_sort_func(sort_func_t sort, 
                    const char *depict, int *array, int len)
{
#ifdef _SHOW_ARRAY
    printf("\nshuffling:\n");
#endif
    shuffling(array, len);
    print_array(array, len);

    time_t start = clock();
    sort(array, len);   
    time_t end = clock();
    
    if (depict) {
        printf("%s: %f\n", depict, (double)(end - start)/CLOCKS_PER_SEC);
    }
    print_array(array, len);
}

int main(int argc, char *argv[])
{
    int array_len = DEFAULT_ARRAY_LEN;

    if (argc > 1 && atoi(argv[1]) > 0) {
        array_len = atoi(argv[1]);
    }
    srand((unsigned int)time(NULL));

    int *array = get_test_array(array_len);
    
    if (NULL == array)
        return -1;

    printf("random array length: %d\n", array_len);
    
    test_sort_func(bubble_sort, "bubble_sort", array, array_len);
    
    test_sort_func(select_sort, "select_sort", array, array_len);
    
    test_sort_func(insert_sort, "insert_sort", array, array_len);
   
    printf("////////////////////////\n");

    test_sort_func(quick_sort, "quick_sort", array, array_len);
    
    test_sort_func(shell_sort, "shell_sort", array, array_len);
   
    test_sort_func(merge_sort, "merge_sort", array, array_len);

    free_test_array(array);
    return 0;
}

