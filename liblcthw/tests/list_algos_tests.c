#include "minunit.h"
#include <lcthw/list_algos.h>
//#include "../src/lcthw/list.h" //only for writing purposes
#include <assert.h>
#include <string.h>
#include <time.h> //for ec

char *values[] = { "XXXX", "1234", "abcd", "xjvef", "NDSS" };

#define NUM_VALUES 5

List *create_words() {
    //int i = 0;
    List *words = List_create();

    for (int i=0; i<NUM_VALUES; i++) {
        List_push(words, values[i]);
    }

    return words;
}

//for extra credit
List *create_big_words() {
    //int i = 0;
    List *words = List_create();

    for (int j=0; j<5000; j++) {
        for (int i=0; i<NUM_VALUES; i++) {
            List_push(words, values[i]);
        }
    }

    return words;
}

int is_sorted(List *words) {
    LIST_FOREACH(words, first, next, cur) {
        if (cur->next && strcmp(cur->value, cur->next->value) > 0) {
            debug("%s %s", (char *)cur->value, (char *)cur->next->value);
            return 0;
        }
    }

    return 1;
}

char *test_bubble_sort_complicated() {
    List *words = create_words();

    // should work on a list that needs sorting
    int rc = List_bubble_sort_complicated(words, (List_compare) strcmp); //we pass strcmp as a function pointer named List_compare (to do in list_algos.h)
    mu_assert(rc == 0, "Bubble sort failed.");
    mu_assert(is_sorted(words), "Words are not sorted after bubble sort.");
    mu_assert(List_count(words) == NUM_VALUES, "Bubble sort didn't return the full list."); //additional tests for len of returned list (shouldn't be a problem here, but common using merge sort)

    // should work on already sorted list
    rc = List_bubble_sort_complicated(words, (List_compare) strcmp);
    mu_assert(rc == 0, "Bubble sort of already sorted failed.");
    mu_assert(is_sorted(words), "Words should still be sorted if already bubble sorted.");
    mu_assert(List_count(words) == NUM_VALUES, "Lenght of list should still be the same after bubble sort on sorted.");

    List_destroy(words);

    // should work on empty list
    //words = List_create(words); //i think his is wrong (List_create takes no arguemnts)
    words = List_create(); //i think it should be like this
    rc = List_bubble_sort_complicated(words, (List_compare) strcmp);
    mu_assert(rc == 0, "Bubble sort  failed on empty list.");
    mu_assert(is_sorted(words), "Words should be sorted if empty.");
    mu_assert(List_count(words) == 0, "Empty list has count more than zero after bubble sort.");

    List_destroy(words);

    return NULL;
}

char *test_bubble_sort() {
    List *words = create_words();

    // should work on a list that needs sorting
    int rc = List_bubble_sort(words, (List_compare) strcmp); //we pass strcmp as a function pointer named List_compare (to do in list_algos.h)
    mu_assert(rc == 0, "Bubble sort failed.");
    mu_assert(is_sorted(words), "Words are not sorted after bubble sort.");
    mu_assert(List_count(words) == NUM_VALUES, "Bubble sort didn't return the full list."); //additional tests for len of returned list (shouldn't be a problem here, but common using merge sort)

    // should work on already sorted list
    rc = List_bubble_sort(words, (List_compare) strcmp);
    mu_assert(rc == 0, "Bubble sort of already sorted failed.");
    mu_assert(is_sorted(words), "Words should still be sorted if already bubble sorted.");
    mu_assert(List_count(words) == NUM_VALUES, "Lenght of list should still be the same after bubble sort on sorted.");

    List_destroy(words);

    // should work on empty list
    //words = List_create(words); //i think his is wrong (List_create takes no arguemnts)
    words = List_create(); //i think it should be like this
    rc = List_bubble_sort(words, (List_compare) strcmp);
    mu_assert(rc == 0, "Bubble sort  failed on empty list.");
    mu_assert(is_sorted(words), "Words should be sorted if empty.");
    mu_assert(List_count(words) == 0, "Empty list has count more than zero after bubble sort.");

    List_destroy(words);

    return NULL;
}

char *test_merge_sort() {
    List *words = create_words();

    // should work on list that needs sorting
    List *res = List_merge_sort(words, (List_compare) strcmp);
    mu_assert(is_sorted(res), "Words are not sorted after merge sort");
    mu_assert(List_count(res) == NUM_VALUES, "Merge sort didn't return the full list.");

    List *res2 = List_merge_sort(res, (List_compare) strcmp);
    mu_assert(is_sorted(res2), "Should still be sorted after merge sort on sorted.");
    mu_assert(List_count(res2) == NUM_VALUES, "Lenght of list should still be the same after merge sort on sorted.");

    List_destroy(res2);
    //List_destroy(res); //don't need to destroy those because our List_merge_sort() does that for us
    //List_destroy(words);
    //may need to uncoment for zeds version

    return NULL;
}

char *all_tests() {
    mu_suite_start();

    mu_run_test(test_bubble_sort_complicated);
    mu_run_test(test_bubble_sort);
    mu_run_test(test_merge_sort);

    // Extra Credit
    printf("-- Extra Credit -- (uncomment in list_algos_tests.c to run (takes a few seconds))\n");
    /*
    // time testing
    clock_t start;
    clock_t end;

    //lots of small sorts
    //bubble_sort_complicated
    start = clock();
    for (int i=0; i<1000000; i++) { //for every algo we run 1000000 * 2 sortings, one unsorted and one already sorted
        List *words = create_words();
        int rc = List_bubble_sort_complicated(words, (List_compare) strcmp);
        rc = List_bubble_sort_complicated(words, (List_compare) strcmp);
        List_destroy(words);
    }
    end = clock();
    printf("many small bubble_sort_complicated: %fs\n", (double)(end-start) / CLOCKS_PER_SEC);

    //bubble_sort
    start = clock();
    for (int i=0; i<1000000; i++) {
        List *words = create_words();
        int rc = List_bubble_sort(words, (List_compare) strcmp);
        rc = List_bubble_sort(words, (List_compare) strcmp);
        List_destroy(words);
    }
    end = clock();
    printf("many small bubble_sort: %fs\n", (double)(end-start) / CLOCKS_PER_SEC);

    //merge_sort
    start = clock();
    for (int i=0; i<1000000; i++) {
        List *words = create_words();
        List *res = List_merge_sort(words, (List_compare) strcmp);
        res = List_merge_sort(res, (List_compare) strcmp);
        //List_destroy(words);
        List_destroy(res);
    }
    end = clock();
    printf("many small merge_sort: %fs\n", (double)(end-start) / CLOCKS_PER_SEC);

    //for multiple sortings of small samples (especially for if they are already sorted) the merge sort acutally isnt that good (has a lot of overhead)

    //one big sort
    //bubble_sort_complicated
    List *big_words = create_big_words();
    start = clock();
    int rc = List_bubble_sort_complicated(big_words, (List_compare) strcmp);
    end = clock();
    List_destroy(big_words);
    printf("one big bubble_sort_complicated: %fs\n", (double)(end-start) / CLOCKS_PER_SEC);

    //bubble_sort
    big_words = create_big_words();
    start = clock();
    rc = List_bubble_sort(big_words, (List_compare) strcmp);
    end = clock();
    List_destroy(big_words);
    printf("one big bubble_sort: %fs\n", (double)(end-start) / CLOCKS_PER_SEC);

    //bubble_sort
    big_words = create_big_words();
    start = clock();
    List *res = List_merge_sort(big_words, (List_compare) strcmp);
    end = clock();
    List_destroy(big_words);
    printf("one big merge_sort: %fs\n", (double)(end-start) / CLOCKS_PER_SEC);

    //but sorting a large list is WAY faster using merge sort
    //also there is an actual difference between complicated and simple bubble_sort (simple is faster, quite a lot)
    */

    return NULL;
}

RUN_TESTS(all_tests);
