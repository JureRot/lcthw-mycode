#ifndef darray_algos_h
#define darray_algos_h

#include <lcthw/darray.h>
//#include "darray.h" //just for writing

typedef int (*DArray_compare) (const void *a, const void *b);

//quick sort on DArray (works on all platforms)
int DArray_qsort(DArray *array, DArray_compare cmp);

/* HEAPSORT AND MERGESORT ONLY WORK ON BSD
//heap sort on DArray (only works on BSD platforms or using libbsd/-lbsd)
int DArray_heapsort(DArray *array, DArray_compare cmp);

//heap sort on DArray (only works on BSD platforms or using libbsd/-lbsd)
int DArray_mergesort(DArray *array, DArray_compare cmp);
*/

//my implementation of quick sort (should work on all platforms)
//get DArray and compare function
//returns 0 on success, -1 on error
int DArray_my_qsort(DArray *array, DArray_compare cmp);

//my implementation of heap sort (should work on all platforms)
//get DArray and compare function
//returns 0 on success, -1 on error
int DArray_my_heapsort(DArray *array, DArray_compare cmp);

//my implementation of merge sort (should work on all platforms)
//get DArray and compare function
//returns 0 on success, -1 on error
int DArray_my_mergesort(DArray *array, DArray_compare cmp);

//finds the index of element in array
//returns index of el in array on success or -1 on error
int DArray_find(DArray *array, void *el, DArray_compare cmp);

//inserts el to already sorted array in a way it remains sorted
//returns 0 on success, -1 on error
int DArray_sort_add(DArray *array, void *el, DArray_compare cmp);


#endif
