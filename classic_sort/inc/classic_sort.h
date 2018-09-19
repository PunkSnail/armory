
#ifndef CLASSIC_SORT_HH
#define CLASSIC_SORT_HH

#define swap_value(a, b) \
    do { a = a ^ b; b = a ^ b; a = a ^ b; } while(0)

#ifdef __cplusplus
extern "C" {
#endif

void bubble_sort(int *array, int len);

void select_sort(int *array, int len);

void insert_sort(int *array, int len);

void quick_sort(int *array, int len);

void shell_sort(int *array, int len);

void merge_sort(int *array, int len);

#ifdef __cplusplus
}
#endif

#endif /* CLASSIC_SORT_HH */
