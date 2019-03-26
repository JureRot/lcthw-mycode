#include "minunit.h"
#include <lcthw/darray_algos.h>
//#include "../src/lcthw/darray_algos.h" //just for writting
//#include "../src/lcthw/darray.h" //just for writting
#include <time.h>

int testcmp(char **a, char **b) {
    return strcmp(*a, *b);
}
//char ** because qsort gives pointer to the pointers in contents
//qsort, heapsort and merge sort are scanning the array and handing pointers to each element in the array to your comparison function. since we have pointers in contents this is a pointer to a pointer (thus char **)

DArray *create_words() {
    DArray *result = DArray_create(0, 5);
    char *words[] = { "asdfasfd", "werwar", "13234", "asdfasfd", "oioj" };

    for (int i=0; i<5; i++) {
        DArray_push(result, words[i]);
    }

    return result;
}

int is_sorted(DArray *array) {
    for (int i=0; i<DArray_count(array)-1; i++) {
        if (strcmp(DArray_get(array, i), DArray_get(array, i+1)) > 0) { //if any first bigger than the second we return 0 as false
            return 0;
        }
    }

    return 1;
}

char *run_sort_test(int (*func) (DArray *, DArray_compare), const char *name) { //func is a function pointer
    DArray *words = create_words();
    mu_assert(!is_sorted(words), "Words should start not sorted.");

    debug("--- Testing %s sorting algorithm ---", name);
    int rc = func(words, (DArray_compare) testcmp);
    mu_assert(rc == 0, "Sort failed.");
    mu_assert(is_sorted(words), "Didn't sort words.");

    DArray_destroy(words);

    return NULL;
}

char *test_qsort() {
    return run_sort_test(DArray_qsort, "qsort");
}

char *test_heapsort() {
    return run_sort_test(DArray_heapsort, "heapsort");
}

char *test_mergesort() {
    return run_sort_test(DArray_mergesort, "mergesort");
}

char *test_my_qsort() {
    return  run_sort_test(DArray_my_qsort, "my_qsort");
}

char *test_my_heapsort() {
    return  run_sort_test(DArray_my_heapsort, "my_heapsort");
}

char *test_my_mergesort() {
    return  run_sort_test(DArray_my_mergesort, "my_mergesort");
}

char *all_tests() {
    mu_suite_start();

    mu_run_test(test_qsort);
    mu_run_test(test_heapsort);
    mu_run_test(test_mergesort);

    // Extra Credit
    mu_run_test(test_my_qsort);
    mu_run_test(test_my_heapsort);
    mu_run_test(test_my_mergesort);

    printf("-- Extra Credit -- (uncomment in darray_algos_tests.c to run (takes a few seconds))\n");

    //timing tests on all sorts
    clock_t start;
    clock_t end;

    //add timing test for all 6 algos both many small sorts and one big

    //printf("fast one big (30000 inserts): %fs\n", (double)(end-start)/CLOCKS_PER_SEC);


    return NULL;
}

RUN_TESTS(all_tests);
