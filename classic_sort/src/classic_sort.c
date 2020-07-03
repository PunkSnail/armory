
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "classic_sort.h"


void bubble_sort(int *array, uint32_t len)
{
    uint32_t lo = 0, hi = len;
    uint32_t last;

    for ( ; lo < hi; hi = last, lo = 0)
    {
        last = lo;

        while (++lo < hi)
        {
            if (array[lo - 1] > array[lo])
            {
                last = lo;
                swap_value(array[lo - 1], array[lo]);
            }
        }
    }
}

void select_sort(int *array, uint32_t len)
{
    uint32_t min_index = 0, i = 0, j = 0;

    for (i = 0; i < len; i++)
    {
        min_index = i;

        for (j = i + 1; j < len; j++)
        {
            if (array[j] < array[min_index]) {
                min_index = j;
            }
        }
        if (min_index != i) {
            swap_value(array[i], array[min_index]);
        }
    }
}

void insert_sort(int *array, uint32_t len)
{
    int tmp = 0;
    uint32_t i = 0, j = 0;

    for (i = 1; i < len; i++)
    {
        tmp = array[i];

        for (j = i; j > 0 && tmp < array[j - 1]; j--) {
            array[j] = array[j - 1];
        }
        array[j] = tmp;
    }
}

void shell_sort(int *array, uint32_t len)
{
    int i = 0, j = 0, k = 0;
    int rule = (int)len;
    int tmp = 0;
    do {
        rule = rule / 3 + 1;

        for (i = 0; i < rule; i++)
        {
            for (j = i + rule; j < (int)len; j += rule)
            {
                if (array[j] < array[j - rule])
                {
                    tmp = array[j];

                    for (k = j - rule; k >= 0 && tmp < array[k]; k -= rule)
                    {
                        array[k + rule] = array[k];
                    }
                    array[k + rule] = tmp;
                }
            }
        }
   } while(rule > 1);
}

static void quick_recursion(int *array, int start, int end)
{
    if (start >= end) {
        return;
    }
    int basis = array[start];

    int i = start;
    int j = end;

    while (i < j)
    {
        while (i < j && array[j] > basis) {
            j--;
        }
        if (i < j)
        {
            array[i] = array[j];
            i++;
        }
        while (i < j && array[i] < basis) {
            i++;
        }
        if (i < j)
        {
            array[j] = array[i];
            j--;
        }
    }
    array[i] = basis;

    quick_recursion(array, start, i - 1);

    quick_recursion(array, i + 1, end);
}

void quick_sort(int *array, uint32_t len)
{
    quick_recursion(array, 0, (int)len - 1);
}

static void merger(int *array, int *arr_buf,
                   uint32_t lo, uint32_t mi, uint32_t hi)
{
    int *l_arr = arr_buf;
    int *r_arr = array + mi;

    uint32_t l_len = mi - lo;
    uint32_t r_len = hi - mi;

    array = array + lo;
    memcpy(l_arr, array, l_len * sizeof(array[0]));

    for (uint32_t i = 0, j = 0, k = 0; j < l_len; )
    {
        if (r_len <= k || l_arr[j] <= r_arr[k])
        {
            array[i++] = l_arr[j++];
        }
        if (k < r_len && r_arr[k] < l_arr[j])
        {
            array[i++] = r_arr[k++];
        }
    }
}

static void merge_recursion(int *array, int *arr_buf,
                            uint32_t lo, uint32_t hi)
{
    if (hi - lo <= 1)
    {
        return;
    }
    uint32_t mi = (lo + hi) >> 1;

    merge_recursion(array, arr_buf, lo, mi);

    merge_recursion(array, arr_buf, mi, hi);

    merger(array, arr_buf, lo, mi, hi);
}

void merge_sort(int *array, uint32_t len)
{
    int *arr_buf = NULL;

    if ( 0 == len || !(arr_buf = (int*)calloc(len, sizeof(array[0]))) )
    {
        return;
    }
    merge_recursion(array, arr_buf, 0, len);

    free(arr_buf);
}

