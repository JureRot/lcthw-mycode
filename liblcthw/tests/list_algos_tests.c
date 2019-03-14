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

char *test_insert_sorted() {
    List *words = create_words();

    int rc = List_bubble_sort(words, (List_compare) strcmp);

    //insert beginning
    List_insert_sorted(words, "01", (List_compare) strcmp);
    mu_assert(List_count(words) == NUM_VALUES+1, "insert_sorted didn't insert value/increase count when inserting to beginning.");
    mu_assert(is_sorted(words), "Words are not sorted after insert_sorted.");
    mu_assert(strcmp(words->first->value, "01") == 0, "insert_sorted didn't insert to right spot at the beginning.");

    //insert end
    List_insert_sorted(words, "zz", (List_compare) strcmp);
    mu_assert(List_count(words) == NUM_VALUES+2, "insert_sorted didn't insert value/increase count when inserting to end.");
    mu_assert(is_sorted(words), "Words are not sorted after insert_sorted.");
    mu_assert(strcmp(words->last->value, "zz") == 0, "insert_sorted didn't insert to right spot at the end.");

    //insert middle
    List_insert_sorted(words, "02", (List_compare) strcmp);
    mu_assert(List_count(words) == NUM_VALUES+3, "insert_sorted didn't insert value/increase count when inserting to middle.");
    mu_assert(is_sorted(words), "Words are not sorted after insert_sorted.");
    mu_assert(strcmp(words->first->next->value, "02") == 0, "insert_sorted didn't insert to right spot in the middle.");

    List_destroy(words);

    return NULL;
}

char *all_tests() {
    mu_suite_start();

    mu_run_test(test_bubble_sort_complicated);
    mu_run_test(test_bubble_sort);
    mu_run_test(test_merge_sort);
    // Extra credit
    mu_run_test(test_insert_sorted);

    // Extra Credit
    // time testing
    printf("-- Extra Credit -- (uncomment in list_algos_tests.c to run (takes a few seconds))\n");
/*
    clock_t start;
    clock_t end;

    //LOTS OF SMALL SORTS
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
    printf("\n");

    //ONE BIG SORT
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
    List_destroy(res);
    printf("one big merge_sort: %fs\n", (double)(end-start) / CLOCKS_PER_SEC);

    //but sorting a large list is WAY faster using merge sort
    //also there is an actual difference between complicated and simple bubble_sort (simple is faster, quite a lot)
    printf("\n");

    //TESTING BIT O NOTATION
    //bubble_sort_complicated
    printf("bubble_sort_complicated:\n");
    //1000
    List *dynamic_words = create_words();
    for (int i=0; i<199; i++) {
        dynamic_words = List_join(dynamic_words, create_words());
    }
    start = clock();
    rc = List_bubble_sort_complicated(dynamic_words, (List_compare) strcmp);
    end = clock();
    List_destroy(dynamic_words);
    printf("\t1000: %fs\n", (double)(end-start) / CLOCKS_PER_SEC);
    //2000
    dynamic_words = create_words();
    for (int i=0; i<399; i++) {
        dynamic_words = List_join(dynamic_words, create_words());
    }
    start = clock();
    rc = List_bubble_sort_complicated(dynamic_words, (List_compare) strcmp);
    end = clock();
    List_destroy(dynamic_words);
    printf("\t2000: %fs\n", (double)(end-start) / CLOCKS_PER_SEC);
    //4000
    dynamic_words = create_words();
    for (int i=0; i<799; i++) {
        dynamic_words = List_join(dynamic_words, create_words());
    }
    start = clock();
    rc = List_bubble_sort_complicated(dynamic_words, (List_compare) strcmp);
    end = clock();
    List_destroy(dynamic_words);
    printf("\t4000: %fs\n", (double)(end-start) / CLOCKS_PER_SEC);
    //8000
    dynamic_words = create_words();
    for (int i=0; i<1599; i++) {
        dynamic_words = List_join(dynamic_words, create_words());
    }
    start = clock();
    rc = List_bubble_sort_complicated(dynamic_words, (List_compare) strcmp);
    end = clock();
    List_destroy(dynamic_words);
    printf("\t8000: %fs\n", (double)(end-start) / CLOCKS_PER_SEC);
    //16000
    dynamic_words = create_words();
    for (int i=0; i<3199; i++) {
        dynamic_words = List_join(dynamic_words, create_words());
    }
    start = clock();
    rc = List_bubble_sort_complicated(dynamic_words, (List_compare) strcmp);
    end = clock();
    List_destroy(dynamic_words);
    printf("\t16000: %fs\n", (double)(end-start) / CLOCKS_PER_SEC);
    //32000
    dynamic_words = create_words();
    for (int i=0; i<6399; i++) {
        dynamic_words = List_join(dynamic_words, create_words());
    }
    start = clock();
    rc = List_bubble_sort_complicated(dynamic_words, (List_compare) strcmp);
    end = clock();
    List_destroy(dynamic_words);
    printf("\t32000: %fs\n", (double)(end-start) / CLOCKS_PER_SEC);
    //64000
    dynamic_words = create_words();
    for (int i=0; i<12799; i++) {
        dynamic_words = List_join(dynamic_words, create_words());
    }
    start = clock();
    rc = List_bubble_sort_complicated(dynamic_words, (List_compare) strcmp);
    end = clock();
    List_destroy(dynamic_words);
    printf("\t64000: %fs\n", (double)(end-start) / CLOCKS_PER_SEC);

    printf("\n");
    //bubble_sort
    printf("bubble_sort:\n");
    //1000
    dynamic_words = create_words();
    for (int i=0; i<199; i++) {
        dynamic_words = List_join(dynamic_words, create_words());
    }
    start = clock();
    rc = List_bubble_sort(dynamic_words, (List_compare) strcmp);
    end = clock();
    List_destroy(dynamic_words);
    printf("\t1000: %fs\n", (double)(end-start) / CLOCKS_PER_SEC);
    //2000
    dynamic_words = create_words();
    for (int i=0; i<399; i++) {
        dynamic_words = List_join(dynamic_words, create_words());
    }
    start = clock();
    rc = List_bubble_sort(dynamic_words, (List_compare) strcmp);
    end = clock();
    List_destroy(dynamic_words);
    printf("\t2000: %fs\n", (double)(end-start) / CLOCKS_PER_SEC);
    //4000
    dynamic_words = create_words();
    for (int i=0; i<799; i++) {
        dynamic_words = List_join(dynamic_words, create_words());
    }
    start = clock();
    rc = List_bubble_sort(dynamic_words, (List_compare) strcmp);
    end = clock();
    List_destroy(dynamic_words);
    printf("\t4000: %fs\n", (double)(end-start) / CLOCKS_PER_SEC);
    //8000
    dynamic_words = create_words();
    for (int i=0; i<1599; i++) {
        dynamic_words = List_join(dynamic_words, create_words());
    }
    start = clock();
    rc = List_bubble_sort(dynamic_words, (List_compare) strcmp);
    end = clock();
    List_destroy(dynamic_words);
    printf("\t8000: %fs\n", (double)(end-start) / CLOCKS_PER_SEC);
    //16000
    dynamic_words = create_words();
    for (int i=0; i<3199; i++) {
        dynamic_words = List_join(dynamic_words, create_words());
    }
    start = clock();
    rc = List_bubble_sort(dynamic_words, (List_compare) strcmp);
    end = clock();
    List_destroy(dynamic_words);
    printf("\t16000: %fs\n", (double)(end-start) / CLOCKS_PER_SEC);
    //32000
    dynamic_words = create_words();
    for (int i=0; i<6399; i++) {
        dynamic_words = List_join(dynamic_words, create_words());
    }
    start = clock();
    rc = List_bubble_sort(dynamic_words, (List_compare) strcmp);
    end = clock();
    List_destroy(dynamic_words);
    printf("\t32000: %fs\n", (double)(end-start) / CLOCKS_PER_SEC);
    //64000
    dynamic_words = create_words();
    for (int i=0; i<12799; i++) {
        dynamic_words = List_join(dynamic_words, create_words());
    }
    start = clock();
    rc = List_bubble_sort(dynamic_words, (List_compare) strcmp);
    end = clock();
    List_destroy(dynamic_words);
    printf("\t64000: %fs\n", (double)(end-start) / CLOCKS_PER_SEC);

    printf("\n");
    //merge_sort
    printf("merge_sort:\n");
    //1000
    dynamic_words = create_words();
    for (int i=0; i<199; i++) {
        dynamic_words = List_join(dynamic_words, create_words());
    }
    start = clock();
    res = List_merge_sort(dynamic_words, (List_compare) strcmp);
    end = clock();
    List_destroy(res);
    printf("\t1000: %fs\n", (double)(end-start) / CLOCKS_PER_SEC);
    //2000
    dynamic_words = create_words();
    for (int i=0; i<399; i++) {
        dynamic_words = List_join(dynamic_words, create_words());
    }
    start = clock();
    res = List_merge_sort(dynamic_words, (List_compare) strcmp);
    end = clock();
    List_destroy(res);
    printf("\t2000: %fs\n", (double)(end-start) / CLOCKS_PER_SEC);
    //4000
    dynamic_words = create_words();
    for (int i=0; i<799; i++) {
        dynamic_words = List_join(dynamic_words, create_words());
    }
    start = clock();
    res = List_merge_sort(dynamic_words, (List_compare) strcmp);
    end = clock();
    List_destroy(res);
    printf("\t4000: %fs\n", (double)(end-start) / CLOCKS_PER_SEC);
    //8000
    dynamic_words = create_words();
    for (int i=0; i<1599; i++) {
        dynamic_words = List_join(dynamic_words, create_words());
    }
    start = clock();
    res = List_merge_sort(dynamic_words, (List_compare) strcmp);
    end = clock();
    List_destroy(res);
    printf("\t8000: %fs\n", (double)(end-start) / CLOCKS_PER_SEC);
    //16000
    dynamic_words = create_words();
    for (int i=0; i<3199; i++) {
        dynamic_words = List_join(dynamic_words, create_words());
    }
    start = clock();
    res = List_merge_sort(dynamic_words, (List_compare) strcmp);
    end = clock();
    List_destroy(res);
    printf("\t16000: %fs\n", (double)(end-start) / CLOCKS_PER_SEC);
    //32000
    dynamic_words = create_words();
    for (int i=0; i<6399; i++) {
        dynamic_words = List_join(dynamic_words, create_words());
    }
    start = clock();
    res = List_merge_sort(dynamic_words, (List_compare) strcmp);
    end = clock();
    List_destroy(res);
    printf("\t32000: %fs\n", (double)(end-start) / CLOCKS_PER_SEC);
    //64000
    dynamic_words = create_words();
    for (int i=0; i<12799; i++) {
        dynamic_words = List_join(dynamic_words, create_words());
    }
    start = clock();
    res = List_merge_sort(dynamic_words, (List_compare) strcmp);
    end = clock();
    List_destroy(res);
    printf("\t64000: %fs\n", (double)(end-start) / CLOCKS_PER_SEC);
    //128000
    dynamic_words = create_words();
    for (int i=0; i<25599; i++) {
        dynamic_words = List_join(dynamic_words, create_words());
    }
    start = clock();
    res = List_merge_sort(dynamic_words, (List_compare) strcmp);
    end = clock();
    List_destroy(res);
    printf("\t128000: %fs\n", (double)(end-start) / CLOCKS_PER_SEC);
    //256000
    dynamic_words = create_words();
    for (int i=0; i<51199; i++) {
        dynamic_words = List_join(dynamic_words, create_words());
    }
    start = clock();
    res = List_merge_sort(dynamic_words, (List_compare) strcmp);
    end = clock();
    List_destroy(res);
    printf("\t256000: %fs\n", (double)(end-start) / CLOCKS_PER_SEC);

    //both bubble sorts have exponential o notation O(n^2)
    //merge sort has theoreticyl O(n*log(n)) but in this program it seems quite linear (O(2n))

    //one more thing -> i can't comfirm this extra credit part has no mem leaks (valgrind is way to slow)
*/

    return NULL;
}
RUN_TESTS(all_tests);

