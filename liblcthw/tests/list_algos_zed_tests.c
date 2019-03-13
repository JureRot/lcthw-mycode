#include "minunit.h"
#include <lcthw/list_algos_zed.h>
//#include "../src/lcthw/list.h" //only for writing purposes
#include <assert.h>
#include <string.h>

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

int is_sorted(List *words) {
    LIST_FOREACH(words, first, next, cur) {
        if (cur->next && strcmp(cur->value, cur->next->value) > 0) {
            debug("%s %s", (char *)cur->value, (char *)cur->next->value);
            return 0;
        }
    }

    return 1;
}

char *test_bubble_sort() {
    List *words = create_words();

    // should work on a list that needs sorting
    int rc = List_bubble_sort_zed(words, (List_compare) strcmp); //we pass strcmp as a function pointer named List_compare (to do in list_algos.h)
    mu_assert(rc == 0, "Bubble sort failed.");
    mu_assert(is_sorted(words), "Words are not sorted after bubble sort.");
    mu_assert(List_count(words) == NUM_VALUES, "Bubble sort didn't return the full list."); //additional tests for len of returned list (shouldn't be a problem here, but common using merge sort)

    // should work on already sorted list
    rc = List_bubble_sort_zed(words, (List_compare) strcmp);
    mu_assert(rc == 0, "Bubble sort of already sorted failed.");
    mu_assert(is_sorted(words), "Words should still be sorted if already bubble sorted.");
    mu_assert(List_count(words) == NUM_VALUES, "Lenght of list should still be the same after bubble sort on sorted.");

    List_destroy(words);

    // should work on empty list
    //words = List_create(words); //i think his is wrong (List_create takes no arguemnts)
    words = List_create(); //i think it should be like this
    rc = List_bubble_sort_zed(words, (List_compare) strcmp);
    mu_assert(rc == 0, "Bubble sort  failed on empty list.");
    mu_assert(is_sorted(words), "Words should be sorted if empty.");
    mu_assert(List_count(words) == 0, "Empty list has count more than zero after bubble sort.");

    List_destroy(words);

    return NULL;
}

char *test_merge_sort() {
    List *words = create_words();

    // should work on list that needs sorting
    List *res = List_merge_sort_zed(words, (List_compare) strcmp);
    mu_assert(is_sorted(res), "Words are not sorted after merge sort");
    mu_assert(List_count(res) == NUM_VALUES, "Merge sort didn't return the full list.");

    List *res2 = List_merge_sort_zed(res, (List_compare) strcmp);
    mu_assert(is_sorted(res2), "Should still be sorted after merge sort on sorted.");
    mu_assert(List_count(res2) == NUM_VALUES, "Lenght of list should still be the same after merge sort on sorted.");

    List_destroy(res2);
    List_destroy(res);
    List_destroy(words);

    return NULL;
}

char *all_tests() {
    mu_suite_start();

    mu_run_test(test_bubble_sort);
    mu_run_test(test_merge_sort);

    return NULL;
}

RUN_TESTS(all_tests);
