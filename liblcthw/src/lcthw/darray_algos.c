#include <lcthw/darray_algos.h>
//#include "darray_algos.h" //just for writtin
#include <stdlib.h>

int DArray_qsort(DArray *array, DArray_compare cmp) {
    qsort(array->contents, DArray_count(array), sizeof(void *), cmp);

    return 0;
}

int DArray_heapsort(DArray *array, DArray_compare cmp) {
    return heapsort(array->contents, DArray_count(array), sizeof(void *), cmp);
}

int DArray_mergesort(DArray *array, DArray_compare cmp) {
    return mergesort(array->contents, DArray_count(array), sizeof(void *), cmp);
}

/*
 * IMPORTANT:
 *
 * heapsort and mergesort are part of BSD libraries and do not work by default on linux
 * (need to install libbsd-dev or something like that, and need to link with -lbsd flag)
 *
 * this should not be needed because qsort is better than both of those
 *
 * for this exercise we will do it on osx or manjaro and than comment out the parts that dont work so it will run on ubuntu
 */
