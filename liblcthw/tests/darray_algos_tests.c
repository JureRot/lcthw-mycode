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

/* HEAPSORT AND MERGESORT ONLY WORK ON BSD
char *test_heapsort() {
    return run_sort_test(DArray_heapsort, "heapsort");
}

char *test_mergesort() {
    return run_sort_test(DArray_mergesort, "mergesort");
}
*/

char *test_my_qsort() {
    return  run_sort_test(DArray_my_qsort, "my_qsort");
}

char *test_my_heapsort() {
    return  run_sort_test(DArray_my_heapsort, "my_heapsort");
}

char *test_my_mergesort() {
    return  run_sort_test(DArray_my_mergesort, "my_mergesort");
}

void push_words(DArray *array, int count) {
    char *words[] = { "asdfasfd", "werwar", "13234", "asdfasfd", "oioj" };

    for (int n=0; n<count; n++) { //we push all 5 words to array count times
        for (int i=0; i<5; i++) {
            DArray_push(array, words[i]);
        }
    }
}

char *test_find() {
    DArray *array = DArray_create(0, 5);
    push_words(array, 1);
    DArray_qsort(array, (DArray_compare) testcmp);
    mu_assert(is_sorted(array), "Should start with sorted array.");

    int rc = DArray_find(array, "oioj", (DArray_compare) testcmp);

    mu_assert(rc == 3, "DArray_find returned wrong number.");
    mu_assert(is_sorted(array), "Array not sorted after find.");

    rc = DArray_find(array, "13234", (DArray_compare) testcmp);
    mu_assert(rc == 0, "Second DArray_find returned wrong number.");
    mu_assert(is_sorted(array), "Array not sorted after second find.");

    DArray_destroy(array);

    return NULL;
}
char *test_sort_add() {
    //create, push_words, sort, add, check end, check sorted, repeat, destroy

    DArray *array = DArray_create(0, 8);
    push_words(array, 1);
    DArray_qsort(array, (DArray_compare) testcmp);

    //add front
    char *a = "111";
    int rc = DArray_sort_add(array, a, (DArray_compare) testcmp);
    mu_assert(rc == 0, "sort_add returned error");
    mu_assert(is_sorted(array), "Should be sorted after sort_add");
    mu_assert(DArray_end(array) == 6, "Should have 6 elements");
    mu_assert(testcmp(DArray_first(array), a) == 0, "new element should be first in array");

    //add end
    char *b = "zzz";
    rc = DArray_sort_add(array, b, (DArray_compare) testcmp);
    mu_assert(rc == 0, "sort_add returned error");
    mu_assert(is_sorted(array), "Should be sorted after sort_add");
    mu_assert(DArray_end(array) == 7, "Should have 6 elements");
    mu_assert(testcmp(DArray_last(array), b) == 0, "new element should be last in array");

    //add middle
    char *c = "444";
    rc = DArray_sort_add(array, c, (DArray_compare) testcmp);
    mu_assert(rc == 0, "sort_add returned error");
    mu_assert(is_sorted(array), "Should be sorted after sort_add");
    mu_assert(DArray_end(array) == 8, "Should have 6 elements");
    mu_assert(testcmp(array->contents[2], c) == 0, "new element should be third in array");

    DArray_destroy(array);

    return NULL;
}

char *all_tests() {
    mu_suite_start();

    mu_run_test(test_qsort);
    //mu_run_test(test_heapsort); //heapsort and merge sort only work on BSD
    //mu_run_test(test_mergesort);

    // Extra Credit
    mu_run_test(test_my_qsort);
    mu_run_test(test_my_heapsort);
    mu_run_test(test_my_mergesort);

    mu_run_test(test_find);
    mu_run_test(test_sort_add);

    printf("-- Extra Credit -- (uncomment in darray_algos_tests.c to run (takes a few seconds))\n");
/*
    //timing tests on all sorts
    DArray *array = NULL;
    clock_t start;
    clock_t end;

    //qsort many small
    start = clock();
    for (int i=0; i<10000; i++) {
        array = DArray_create(0, 5); //create array
        push_words(array, 1); //push some values to it
        DArray_qsort(array, (DArray_compare) testcmp); //sort
        DArray_destroy(array); //and cleanup
    }
    end = clock();
    printf("qsort many small (10000 sorts of 5 items): %fs\n", (double)(end-start)/CLOCKS_PER_SEC);
    //will have some overhead from all create and destroy

    //qsort one big
    array = DArray_create(0, 50000);
    push_words(array, 10000);
    start = clock();
    DArray_qsort(array, (DArray_compare) testcmp);
    end = clock();
    DArray_destroy(array);
    printf("qsort one big (1 sort of 50000 items): %fs\n", (double)(end-start)/CLOCKS_PER_SEC);

    //heapsort many small
    start = clock();
    for (int i=0; i<10000; i++) {
        array = DArray_create(0, 5);
        push_words(array, 1);
        DArray_heapsort(array, (DArray_compare) testcmp);
        DArray_destroy(array);
    }
    end = clock();
    printf("heapsort many small (10000 sorts of 5 items): %fs\n", (double)(end-start)/CLOCKS_PER_SEC);
    //will have some overhead from all create and destroy

    //heapsort one big
    array = DArray_create(0, 50000);
    push_words(array, 10000);
    start = clock();
    DArray_heapsort(array, (DArray_compare) testcmp);
    end = clock();
    DArray_destroy(array);
    printf("heapsort one big (1 sort of 50000 items): %fs\n", (double)(end-start)/CLOCKS_PER_SEC);

    //mergesort many small
    start = clock();
    for (int i=0; i<10000; i++) {
        array = DArray_create(0, 5);
        push_words(array, 1);
        DArray_mergesort(array, (DArray_compare) testcmp);
        DArray_destroy(array);
    }
    end = clock();
    printf("mergesort many small (10000 sorts of 5 items): %fs\n", (double)(end-start)/CLOCKS_PER_SEC);
    //will have some overhead from all create and destroy

    //mergesort one big
    array = DArray_create(0, 50000);
    push_words(array, 10000);
    start = clock();
    DArray_mergesort(array, (DArray_compare) testcmp);
    end = clock();
    DArray_destroy(array);
    printf("mergesort one big (1 sort of 50000 items): %fs\n", (double)(end-start)/CLOCKS_PER_SEC);

    //my_qsort many small
    start = clock();
    for (int i=0; i<10000; i++) {
        array = DArray_create(0, 5);
        push_words(array, 1);
        DArray_my_qsort(array, (DArray_compare) testcmp);
        DArray_destroy(array);
    }
    end = clock();
    printf("my_qsort many small (10000 sorts of 5 items): %fs\n", (double)(end-start)/CLOCKS_PER_SEC);
    //will have some overhead from all create and destroy

    //my_qsort one big
    array = DArray_create(0, 50000);
    push_words(array, 10000);
    start = clock();
    DArray_my_qsort(array, (DArray_compare) testcmp);
    end = clock();
    DArray_destroy(array);
    printf("my_qsort one big (1 sort of 50000 items): %fs\n", (double)(end-start)/CLOCKS_PER_SEC);

    //my_heapsort many small
    start = clock();
    for (int i=0; i<10000; i++) {
        array = DArray_create(0, 5);
        push_words(array, 1);
        DArray_my_heapsort(array, (DArray_compare) testcmp);
        DArray_destroy(array);
    }
    end = clock();
    printf("my_heapsort many small (10000 sorts of 5 items): %fs\n", (double)(end-start)/CLOCKS_PER_SEC);
    //will have some overhead from all create and destroy

    //my_heapsort one big
    array = DArray_create(0, 50000);
    push_words(array, 10000);
    start = clock();
    DArray_my_heapsort(array, (DArray_compare) testcmp);
    end = clock();
    DArray_destroy(array);
    printf("my_heapsort one big (1 sort of 50000 items): %fs\n", (double)(end-start)/CLOCKS_PER_SEC);

    //my_mergesort many small
    start = clock();
    for (int i=0; i<10000; i++) {
        array = DArray_create(0, 5);
        push_words(array, 1);
        DArray_my_mergesort(array, (DArray_compare) testcmp);
        DArray_destroy(array);
    }
    end = clock();
    printf("my_mergesort many small (10000 sorts of 5 items): %fs\n", (double)(end-start)/CLOCKS_PER_SEC);
    //will have some overhead from all create and destroy

    //my_mergesort one big
    array = DArray_create(0, 50000);
    push_words(array, 10000);
    start = clock();
    DArray_my_mergesort(array, (DArray_compare) testcmp);
    end = clock();
    DArray_destroy(array);
    printf("my_mergesort one big (1 sort of 50000 items): %fs\n", (double)(end-start)/CLOCKS_PER_SEC);

    //RESULTS
    //already made algos are better than mine (no surprise here)
    //especially my_qsort (segfaults on big arrays)
    //mergesort is great for big arrays
    //heapsort is a bit worse than both because it uses heap (other data structure)
    //quick sort is all around good
    //
    //in general qsort is the best to use (dont know if implements any imporovements (insertion sort on small, choosing pivot, ...))
*/


    return NULL;
}

RUN_TESTS(all_tests);
