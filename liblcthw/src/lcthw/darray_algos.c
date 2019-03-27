#include <lcthw/darray_algos.h>
//#include "darray_algos.h" //just for writtin
#include <stdlib.h>

int DArray_qsort(DArray *array, DArray_compare cmp) {
    qsort(array->contents, DArray_count(array), sizeof(void *), cmp);

    return 0;
}

/* HEAPSORT AND MERGESORT ONLY WORK ON BSD
int DArray_heapsort(DArray *array, DArray_compare cmp) {
    return heapsort(array->contents, DArray_count(array), sizeof(void *), cmp);
}

int DArray_mergesort(DArray *array, DArray_compare cmp) {
    return mergesort(array->contents, DArray_count(array), sizeof(void *), cmp);
}
*/

// EXTRA CREDIT

void my_swap(void **array, int a, int b) {
    void * temp = array[a];
    array[a] = array[b];
    array[b] = temp;
}

//return 0 on success
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

//puts the first element (the top of the heap) in its correct place
//array: array of pointers
//i: node in interest
//n: number of elements in array
//cmp: compare function
void my_heapify(void **array, int i, int n, DArray_compare cmp) {
    int l = 2*i + 1; //we get the location of left and right if represented as heap
    int r = 2*i + 2;
    int max = i; //we set i as max (for now)

    if ((l < n) && cmp(&array[l], &array[max]) > 0) { //if l still inside array (not past the end) and bigger than max (here we are talking about elements at that index, not the values of l and max itself) (i at this point) we set max to l
        max = l;
    }

    if ((r < n) && cmp(&array[r], &array[max]) > 0) { //if r still inside array and bigger than max (i or l at this point) we set max to r
        max = r;
    }

    if (max != i) { //if we actually need to change the heap (i isnt the biggest)
        my_swap(array, i, max); //we swap the i with max
        my_heapify(array, max, n, cmp);
        //if we swaped we may need to push the node further down, so we call heapify on it
    }
}

//builds max heap from array by heapifying from bottom up
//array: array of pointers
//n: number of elements in array
//cmp: compare function
void build_max_heap(void **array, int n, DArray_compare cmp) {
    //we only need to heapify from the middle of the list to the top
    //the second half of heap are all leaves so they dont need to be heapifyed
    //we can just divide here and cast to int (same as floor(), which is what we need)

    for (int half=n/2; half>=0; half--) {
        my_heapify(array, half, n, cmp);
    }
}

int DArray_my_heapsort(DArray *array, DArray_compare cmp) {
    check(array, "Can't use my_heapsort using compare function on NULL array.");
    check(cmp, "Can't use my_heapsort using NULL compare function on array.");

    int n = array->end; //get the number of elements

    //we build max heap from array
    build_max_heap(array->contents, n, cmp);

    //so now te bigest element in array is on top of the heap (which is the beginning of array)
    //we swap that element with the last in the array and now it is in the right spot
    //now we need to heapify the first/top element because it is out of place
    //after heapifying the next largest element will be in the front
    //we repeat till for all elements

    for (n--; n>0; n--) { //we go from the last element to the second (fist will be automatically sorted) and "shorten" the array each time (decrease the n) (n inited above)
        my_swap(array->contents, 0, n); //swap the first (the biggest) and the last element. now he biggest element is in its place
        my_heapify(array->contents, 0, n, cmp); //we heapify the array up until the biggest element (we leave IT alone)
        //this basically brings the next biggest element to the front
    }

    return 0;

error:
    return -1;
}

#define min(a, b) (((a)<(b)) ? (a) : (b))

void my_merge_parts(void **array, int first, int second, int after, void **dest, DArray_compare cmp) {
    int i = first;
    int end_of_first = second; //end_of_second is after

    if (second == after) { //there is noting in second array
        while (i<second) { //we write everything in first to dest (should be sorted)
            dest[i++] = array[first++];
        }
        return; //and return to break
    }
    //this is ran when there is nothing in the second array
    //this is probably when the array is not the square of 2 and there is some remaining elements at the end which are not enough to fill two of these arrays (maybe not even one)

    while ((first < end_of_first) && (second < after)) {
        if (cmp(&array[first], &array[second]) <= 0) { //if first smaller or equal
            dest[i++] = array[first++];
        } else { //if second smaller
            dest[i++] = array[second++];
        }
    }

    //we need to add the remaining of the nonempty part to dest
    if (first == end_of_first) { //if first ran out of elements
        while (second < after) {
            dest[i++] = array[second++];
        }

    } else { //if second ran out of elements
        while (first < end_of_first) {
            dest[i++] = array[first++];
        }
    }

}

int DArray_my_mergesort(DArray *array, DArray_compare cmp) {
    check(array, "Can't use my_mergesort using compare function on NULL array.");
    check(cmp, "Can't use my_mergesort using NULL compare function on array.");

    for (int i=1; i<=array->end; i*=2) {
        void **dest = calloc(array->max, sizeof(void *)); //we reserve place for sorted

        for (int j=0; j<array->end; j+=2*i) {
            my_merge_parts(array->contents, j, min(j+i, array->end), min(j+2*i, array->end), dest, cmp);
        }

        free(array->contents); //free the original contents
        array->contents = dest; //and rewire the new [partly] sorted one
    }

    //i is the width of the subarrays we are merging
    //we start with 1 (as individual elements)
    //and increase by *2 (2, 4, 8, ...) until only one can cover the whole array
    //
    //j is the beginning of the first subarray currently merging
    //it increases by 2*i because we always merge two subarrays
    //
    //inside nested for loop we call merge_parts
    //it takes:
    //    the original array (->contents),
    //    three ints:
    //        start of first subarray
    //        first element after first / start of second subarray
    //        fist element after second
    //    dest array where we will put the new (partaly) sorted array
    //    and cmp function
    //
    //after nested for loop we free the original ->contents and replace it with dest array

    return 0;

error:
    return -1;
}

//add DArray_find that uses binary search on sorted darray to return (i guess) the index
//add DArray_sort_add that inserts an element intto sorted array and keeps it sorted (can use find to sort only the minimum ammount)
