
#include <stdbool.h>
#include <stdlib.h>
#include "classic_sort.h"


void bubble_sort(int *array, int len)
{
    bool sort_flag = false;

    int i = 0, j = 0;

    for (i = 0; i < len && false == sort_flag; i++)
    {
        sort_flag = true;
        
        for (j = len - 1; j > i; j--) 
        {
            if (array[j] < array[j - 1])
            {
                sort_flag = false;
                swap_value(array[j], array[j - 1]);
            }
        }
    }
}

void select_sort(int *array, int len)
{
    int min_index = 0, i = 0, j = 0;

    for (i = 0; i < len; i++)
    {
        min_index = i;

        for (j = i; j < len; j++)
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

void insert_sort(int *array, int len)
{
    int tmp = 0, i = 0, j = 0;

    for (i = 1; i < len; i++)
    {
        if (array[i] < array[i - 1])
        {
            tmp = array[i];
            
            for (j = i - 1; j >= 0 && tmp < array[j]; j--) {
                array[j + 1] = array[j];
            }
            array[j + 1] = tmp;
        }
    }
}

void shell_sort(int *array, int len)
{
    int i = 0, j = 0, k = 0;
    int rasie = len, tmp = 0;
    do {
        rasie = rasie / 3 + 1;
        
        for (i = 0; i < rasie; i++)
        {
            for (j = i + rasie; j < len; j += rasie)
            {
                if (array[j] < array[j - rasie])
                {
                    tmp = array[j];
                    
                    for (k = j - rasie; k >= 0 && tmp < array[k]; k -= rasie) {
                        array[k + rasie] = array[k];
                    }
                    array[k + rasie] = tmp;
                }
            }
        }
   } while(rasie > 1);
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

void quick_sort(int *array, int len)
{
    quick_recursion(array, 0, len - 1);
}

static void merge(int *array, int start, int end, int mid, int *assist_array)
{
    int i_start = start;
    int i_end   = mid;
    int j_start = mid + 1;
    int j_end   = end;
    int index   = 0;

    while (i_start <= i_end && j_start <= j_end)
    {
        if (array[i_start] < array[j_start])
        {
            assist_array[index] = array[i_start];
            i_start++;
        }
        else{
            assist_array[index] = array[j_start];
            j_start++;
        }
        index++;
    }
    while (i_start <= i_end)
    {
        assist_array[index] = array[i_start];
        i_start++;
        index++;
    }
    while (j_start <= j_end)
    {
        assist_array[index] = array[j_start];
        j_start++;
        index++;
    }
    for (int i = 0; i < index; i++) {
        array[start + i] = assist_array[i];
    }
}

static void merge_recursion(int *array, int start, int end, int *assist_array)
{
    if (start == end) {
        return;
    }
    int mid = (start + end) / 2;
    
    merge_recursion(array, start, mid, assist_array);
    
    merge_recursion(array, mid + 1, end, assist_array);
    
    merge(array, start, end, mid, assist_array);
}

void merge_sort(int *array, int len)
{
    int *assist_array = (int*)calloc((size_t)len, sizeof(int));

    if (NULL == assist_array)
        return;

    merge_recursion(array, 0, len -1, assist_array);

    free(assist_array);
    assist_array = NULL;
}

