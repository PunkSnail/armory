
#ifndef CLASSIC_SORT_HH
#define CLASSIC_SORT_HH

#define swap_value(a, b) \
    do { a = a ^ b; b = a ^ b; a = a ^ b; } while(0)

#ifdef __cplusplus
extern "C" {
#endif

/* average: O(n^2)  best: O(n)  worst: O(n^2) */
void bubble_sort(int *array, int len);

/* average: O(n^2)  best: O(n^2)    worst: O(n^2) */
void select_sort(int *array, int len);

/* average: O(n^2)  best: O(n)  worst: O(n^2) */
void insert_sort(int *array, int len);

/* average: O(nlogn)    best: O(nlogn)  worst: O(n^2) */
void quick_sort(int *array, int len);

/* best: O(nlogn)  worst: O(n^2) */
void shell_sort(int *array, int len);

/* average: O(nlogn)    best: O(nlogn)  worst: O(nlogn) */
void merge_sort(int *array, int len);

#ifdef __cplusplus
}
#endif

#endif /* CLASSIC_SORT_HH */
