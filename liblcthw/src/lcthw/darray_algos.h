#ifndef darray_algos_h
#define darray_algos_h

#include <lcthw/darray.h>
//#include "darray.h" //just for writing

typedef int (*DArray_compare) (const void *a, const void *b);

//quick sort on DArray (works on all platforms)
int DArray_qsort(DArray *array, DArray_compare cmp);

//heap sort on DArray (only works on BSD platforms or using libbsd/-lbsd)
int DArray_heapsort(DArray *array, DArray_compare cmp);

//heap sort on DArray (only works on BSD platforms or using libbsd/-lbsd)
int DArray_mergesort(DArray *array, DArray_compare cmp);

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

#endif
