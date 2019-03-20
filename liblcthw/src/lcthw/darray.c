#include <lcthw/darray.h>
//#include "darray.h" //just for writing
//#include "dbg.h" //just for writting
#include <assert.h>

//creates darray of initial_max elements of size element_size
//element_size is the sizeof() of the type of data that will be in the contents
//returns pointer to that darray
DArray *DArray_create(size_t element_size, size_t initial_max) {
    //create space for darray and set max to initial max (check for all the errors)
    DArray *array = malloc(sizeof(DArray));
    check_mem(array);
    array->max = initial_max;
    check(array->max > 0, "You must set initial_max > 0.");

    //create zero initialized space for initial_max number of (void) pointers (and check for all the errors)
    array->contents = calloc(initial_max, sizeof(void *));
    check_mem(array->contents);

    //set error, element_size and expand_rate
    array->end = 0;
    array->element_size = element_size;
    array->expand_rate = DEFAULT_EXPAND_RATE;

    return array;

error:
    if (array) {
        free(array);
    }
    return NULL;
}

//goes over the whole array and frees every value in contents that is not NULL
void DArray_clear(DArray *array) {
    //int i = 0;
    if (array->element_size > 0) {
        for (int i=0; i < array->max; i++) {
            if (array->contents[i] != NULL) {
                free(array->contents[i]);
            }
        }
    }
}

//helping inline function for resizing
//checks if newsize is valid
//reallocs new space for contents of size(max * sizeof(void *)) (and checks for errors)
//    realloc(ptr, size) changes the size of the memory block pointed to by ptr and returns the pointer to the newly allocated space
//    effectivelly allocated a new block of memory of the new size and fills it with the values that are already in the memory block pointed to by ptr
//    realloc does not initialize the expanded memory
//and assigns array->contents to the new location
//returns 0 on success, -1 on error
static inline int DArray_resize(DArray *array, size_t newsize) {
    array->max = newsize;
    check(array->max > 0, "The newsize must be more thant zero.");

    void *contents = realloc(array->contents, array->max * sizeof(void *));
    // check contents and assume realloc doesn't hurt the original on error

    check_mem(contents);

    array->contents = contents;

    return 0;

error:
    return -1;
}

//expands the array to old_max + expand_rate (using resize help inline func) and sets the memory from old_max to new_max to zero
//returns 0 on success, -1 on error
int DArray_expand(DArray *array) {
    size_t old_max = array->max;
    check(DArray_resize(array, array->max + array->expand_rate) == 0, "Failed to expand array to new size: %d", array->max + (int)array->expand_rate);

    memset(array->contents + old_max, 0, array->expand_rate + 1);

    return 0;

error:
    return -1;
}

//contracts the array to max(end, expand_rate) (using resize help inline func)
//    if expand rate bigger than index of last elemen, we set to expand rate,
//    if end bigger (there is more elements than expand_rate), we set to the min size of array (which is end (number of elements in it))
//returns o on success, -1 on error
int DArray_contract(DArray *array) {
    int new_size = array->end < (int)array->expand_rate ? (int)array->expand_rate : array->end;

    return DArray_resize(array, new_size + 1);
}

//frees contents block and the array itself
void DArray_destroy(DArray *array) {
    if (array) {
        if (array->contents) {
            free(array->contents);
        }
        free(array);
    }
}

//clears the contents and destroy the contents and array
void DArray_clear_destroy(DArray *array) {
    DArray_clear(array);
    DArray_destroy(array);
}

//pushes el element to the end of the array
//increases the end (number of elements)
//and expands the array if neccessary (if end/num elements equal or greater than max elements)
//returns 0 on success, -1 on error (from DArray_expand)
int DArray_push(DArray *array, void *el) {
    array->contents[array->end] = el;
    array->end++;

    if (DArray_end(array) >= DArray_max(array)) {
        return DArray_expand(array);
    } else {
            return 0;
    }
}

//removes the last element in array
//decreases the end (number of elements)
//and contracts the array if neccessary (I DONT UNDERSTAND THE MODULO HERE)
//retuns value of popped element on success, NULL on error (problem if value of element is NULL, we can't detect error)
void *DArray_pop(DArray *array) {
    check(array->end - 1 >= 0, "Attempt to pop from empty darray.");

    void *el = DArray_remove(array, array->end - 1);
    array->end--;

    if (DArray_end(array) > (int)array->expand_rate && DArray_end(array) % array->expand_rate) { //why is the modulo here (if multple of expand_rate, we dont contract???)
        DArray_contract(array);
    }

    return el;

error:
    return NULL;
}

//basically we have nondynamic array (a block of memory filled with pointers to the actual values)encasted in out strucutre
//this structure allows us to expand and contract this nondynamic array so that it appears dynamic
//it also allows us to acces and fill the array and all that jazz
