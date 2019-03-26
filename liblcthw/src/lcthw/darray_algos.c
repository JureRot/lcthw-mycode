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

void my_swap(void **array, int a, int b) {
    void * temp = array[a];
    array[a] = array[b];
    array[b] = temp;
}

int my_qsort_recur(void **array, int lo, int hi, int min, int max, DArray_compare cmp) {
    if (lo >= hi) { //if only one element, we have noting to sort
        return 0;
    }

    int pivot = hi--; //get the pivot and make hi the one element before

    while (lo <= hi) { //until lo and hi overlap/cross
        while ((cmp(&array[lo], &array[pivot]) <= 0) && (lo < max)) { //find the lo
            lo++;
        }
        while ((cmp(&array[hi], &array[pivot]) > 0) && (hi > min)) { //find the hi
            hi--;
        }

        if (lo >= hi) { //if lo and hi crossed, we dont swap, just break
            break;
        }

        my_swap(array, lo, hi); //swap hi and low
    }

    my_swap(array, lo, pivot); //swap low and pivot

    //LO IS THE LOCATION OF THE PIVOT AFTER THE SWAP
    //for the lower part hi and max is now lo-1, and new lo is min
    my_qsort_recur(array, min, lo-1, min, lo-1, cmp); //recursive call of the part left/smaller of pivot
    //for the higher part lo and min is now lo+1, and the new hi is max
    my_qsort_recur(array, lo+1, max, lo+1, max, cmp); //recursive call of the part right/bigger of pivot

    return 0;
}

int DArray_my_qsort(DArray *array, DArray_compare cmp) {
    check(array, "Can't use my_qsort using compare function on NULL array.");
    check(cmp, "Can't use my_qsort using NULL compare function on array.");

    //call recrusive qsort on whole contents array in DArray; lo=0, hi=end-1
    return my_qsort_recur(array->contents, 0, array->end-1, 0, array->end-1, cmp);

error:
    return -1;
}

int DArray_my_heapsort(DArray *array, DArray_compare cmp) {
    return -1;
}

int DArray_my_mergesort(DArray *array, DArray_compare cmp) {
    return -1;
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
