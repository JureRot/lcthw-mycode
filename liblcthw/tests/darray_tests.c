#include "minunit.h"
#include <lcthw/darray.h>
//#include "../src/lcthw/darray.h" //for writing only

static DArray *array = NULL;
static int *val1 = NULL;
static int *val2 = NULL;

char *test_create() {
    array = DArray_create(sizeof(int), 100);
    mu_assert(array != NULL, "DArray_create failed.");
    mu_assert(array->contents != NULL, "contents are wrong in darray");
    mu_assert(array->end == 0, "end isn't at the right spot");
    mu_assert(array->element_size == sizeof(int), "element size is wrong");
    mu_assert(array->max == 100, "wrong max length on initial size");

    return NULL;
}

char *test_destroy() {
    DArray_destroy(array);

    return NULL;
}

char *test_new() {
    val1 = DArray_new(array);
    mu_assert(val1 != NULL, "failed to make a new element");

    val2 = DArray_new(array);
    mu_assert(val2 != NULL, "failed to make second new element");

    return NULL;
}

char *test_set() {
    DArray_set(array, 0, val1);
    DArray_set(array, 1, val2);

    return NULL;
}

char *test_get() {
    mu_assert(DArray_get(array, 0) == val1, "wrong first value");
    mu_assert(DArray_get(array, 1) == val2, "wrong second value");

    return NULL;
}

char *test_remove() {
    int *val_check = DArray_remove(array, 0);
    mu_assert(val_check != NULL, "should not get NULL");
    mu_assert(*val_check == *val1, "should get the first value");
    mu_assert(DArray_get(array, 0) == NULL, "first should be gone");
    DArray_free(val_check);

    val_check = DArray_remove(array, 1);
    mu_assert(val_check != NULL, "should not get NULL");
    mu_assert(*val_check == *val2, "should get the first value");
    mu_assert(DArray_get(array, 1) == NULL, "second should be gone");
    DArray_free(val_check);

    return NULL;
}

char *test_expand_contract() {
    int old_max = array->max;
    DArray_expand(array);
    mu_assert((unsigned int)array->max == old_max + array->expand_rate, "wrong size after expand");

    DArray_contract(array);
    mu_assert((unsigned int)array->max == array->expand_rate + 1, "should stay at the expand_rate at least");

    DArray_contract(array);
    mu_assert((unsigned int)array->max == array->expand_rate + 1, "should stay at the expand_rate at least");

    return NULL;
}

char *test_push_pop() {
    //int i = 0;
    for (int i=0; i<1000; i++) {
        int *val = DArray_new(array);
        *val = i * 333;
        DArray_push(array, val);
    }

    mu_assert(array->max == 1201, "wrong max size");

    for (int i=999; i>=0; i--) {
        int *val = DArray_pop(array);
        mu_assert(val != NULL, "shouldn't get a NULL");
        mu_assert(*val == i*333, "wrong value");
        DArray_free(val);
    }

    return NULL;
}

//ec
char *test_first_last() {
    //create darray
    array = DArray_create(sizeof(int), 100);
    //push some values
    for (int i=1; i<4; i++) {
        int *val = DArray_new(array);
        *val = i;
        DArray_push(array, val);
    }

    mu_assert(*(int *)DArray_first(array) == 1, "First value is not right.");
    mu_assert(*(int *)DArray_get(array, 1) == 2, "Second value is not right.");
    mu_assert(*(int *)DArray_last(array) == 3, "Last value is not right.");
    //need to do *(int *) because first, last and get return a void * to the element so we need to cast it to an actual poiter type (int *) and get the value of it *

    DArray_clear_destroy(array);

    return NULL;
}

char *test_end_count_max() {
    //create darray
    array = DArray_create(sizeof(int), 100);
    //push some values
    for (int i=1; i<4; i++) {
        int *val = DArray_new(array);
        *val = i;
        DArray_push(array, val);
    }

    mu_assert(DArray_end(array) == 3, "Wrong end value.");
    mu_assert(DArray_count(array) == 3, "Wrong count value.");
    mu_assert(DArray_max(array) == 100, "Wrong max value.");

    DArray_clear_destroy(array);

    return NULL;
}

char *all_tests() {
    mu_suite_start();

    mu_run_test(test_create);
    mu_run_test(test_new);
    mu_run_test(test_set);
    mu_run_test(test_get);
    mu_run_test(test_remove);
    mu_run_test(test_expand_contract);
    mu_run_test(test_push_pop);
    mu_run_test(test_destroy);
    // Extra Credit
    mu_run_test(test_first_last);
    mu_run_test(test_end_count_max);

    //other Extra Credit
    //there should be some other performance tests using time.h but that is just alot of wirting and i've done something like that list_alogs_tests so i don't want do do it again
    //another thing is to have darray_expand and darray_contract work not by static amoutn but by 2x or something like that and again to test that (again something i dont have the will to do rn)

    return NULL;
}

RUN_TESTS(all_tests);
