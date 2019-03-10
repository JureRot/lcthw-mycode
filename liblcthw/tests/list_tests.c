#include "minunit.h"
#include <lcthw/list.h>
//#include "../src/lcthw/list.h"
#include <assert.h>

static List *list = NULL;
List *copy = NULL;
List *a = NULL;
List *b = NULL;
char *test1 = "test1 data";
char *test2 = "test2 data";
char *test3 = "test3 data";

char *test_create() {
    list = List_create();
    mu_assert(list != NULL, "Failed to create list.");

    return NULL;
}

char *test_destroy() {
    List_clear_destroy(list);

    return NULL;
}

char *test_push_pop() {
    List_push(list, test1);
    mu_assert(List_last(list) == test1, "Wrong last value.");

    List_push(list, test2);
    mu_assert(List_last(list) == test2, "Wrong last value.");

    List_push(list, test3);
    mu_assert(List_last(list) == test3, "Wrong last value.");
    mu_assert(List_count(list) == 3, "Wrong count on push.");

    char *val = List_pop(list);
    mu_assert(val == test3, "Wrong value on pop.");

    val = List_pop(list);
    mu_assert(val == test2, "Wrong value on pop.");

    val = List_pop(list);
    mu_assert(val == test1, "Wrong value on pop.");
    mu_assert(List_count(list) == 0, "Wrong count after pop.");

        return NULL;
}

char *test_unshift() {
    List_unshift(list, test1);
    mu_assert(List_first(list) == test1, "Wrong first value.");

    List_unshift(list, test2);
    mu_assert(List_first(list) == test2, "Wrong first value.");

    List_unshift(list, test3);
    mu_assert(List_first(list) == test3, "Wrong first value.");
    mu_assert(List_count(list) == 3, "Wrong count on unshift.");

    return NULL;
}

char *test_remove() {
    // we only need to test the middle remove since push/shift already tests the other cases

    char *val = List_remove(list, list->first->next);
    mu_assert(val == test2, "Wrong removed element.");
    mu_assert(List_count(list) == 2, "Wrong count after remove.");
    mu_assert(List_first(list) == test3, "Wrong first after remove.");
    mu_assert(List_last(list) == test1, "Wrong last after remove.");

    return NULL;
}

char *test_shift() {
    mu_assert(List_count(list) != 0, "Wrong count before shift.");

    char *val = List_shift(list);
    mu_assert(val == test3, "Wrong value on shift.");

    val = List_shift(list);
    mu_assert(val == test1, "Wrong value on shift.");
    mu_assert(List_count(list) == 0, "Wrong count after shift.");

    return NULL;
}

char *test_copy() {
    mu_assert(List_count(list) != 0, "Wrong count before copy");

    int list_cnt = List_count(list);
    char *list_first = List_first(list);
    char *list_last = List_last(list);

    copy = List_copy(list);

    mu_assert(list_cnt == List_count(list), "Count of original changed.");
    mu_assert(list_first == List_first(list), "First element of original changed");
    mu_assert(list_last == List_last(list), "Last element of original changed");

    mu_assert(list_cnt == List_count(copy), "Count of copy doesn't match.");
    mu_assert(list_first == List_first(copy), "First element of copy doesn't match.");
    mu_assert(list->first->next->value == copy->first->next->value, "Second element of copy doesn't match.");
    mu_assert(list_last == List_last(copy), "Last element of copy doesns't match.");

    //FIXME
    List_destroy(copy); //doing List_clear_destroy() here caused segfault (even with default slow version)

    return NULL;
}

char *test_join() {
    a = List_create();
    b = List_create();

    List_push(a, test1);
    List_push(a, test2);
    List_push(a, test3);

    List_push(b, test2);
    List_push(b, test3);
    List_push(b, test1);

    mu_assert(a->count == 3, "Failed creating list a.");
    mu_assert(b->count == 3, "Failed creating list b.");

    a = List_join(a, b);

    mu_assert(a->count == 6, "Wrong count of joined list.");

    mu_assert(List_first(a) == test1, "Wrong first element of joined list.");
    mu_assert(List_last(a) == test1, "Wrong last element of joined list.");

    //tedious check of all the values (forward and back)
    mu_assert(a->first->value == test1, "Wrong first element of joined list.");
    mu_assert(a->first->next->value == test2, "Wrong second element of joined list.");
    mu_assert(a->first->next->next->value == test3, "Wrong third element of joined list.");
    mu_assert(a->first->next->next->next->value == test2, "Wrong fourth element of joined list.");
    mu_assert(a->first->next->next->next->next->value == test3, "Wrong fifth element of joined list.");
    mu_assert(a->first->next->next->next->next->next->value == test1, "Wrong sixth element of joined list.");

    mu_assert(a->last->value == test1, "Wrong last element of joined list.");
    mu_assert(a->last->prev->value == test3, "Wrong second to last element of joined list.");
    mu_assert(a->last->prev->prev->value == test2, "Wrong third to last element of joined list.");
    mu_assert(a->last->prev->prev->prev->value == test3, "Wrong fourth to last element of joined list.");
    mu_assert(a->last->prev->prev->prev->prev->value == test2, "Wrong fifth to last element of joined list.");
    mu_assert(a->last->prev->prev->prev->prev->prev->value == test1, "Wrong sixth to last element of joined list.");

    List_destroy(a);
    //b is already destroyed (freed inside the List_join() function)

    return NULL;
}

char *test_split() {
    a = List_create();
    b = NULL;

    List_push(a, test1);
    List_push(a, test2);
    List_push(a, test3);

    mu_assert(List_count(a) == 3, "Failed creating list a.");

    b = List_split(a, a->first->next);

    mu_assert(List_count(a) == 1, "Wrong count of first list after split.");
    mu_assert(List_count(b) == 2, "Wrong count of second list after split.");

    mu_assert(List_first(a) == test1, "Wrong first item of first list after split.");
    mu_assert(List_last(a) == test1, "Wrong last item of first list after split.");

    mu_assert(List_first(b) == test2, "Wrong first item of second list after split.");
    mu_assert(List_last(b) == test3, "Wrong last item of second list after split.");

    //test a second time on the last element
    a = List_join(a, b);
    b = List_split(a, a->last);

    mu_assert(List_count(a) == 2, "Wrong count of first list after second split.");
    mu_assert(List_count(b) == 1, "Wrong count of second list after second split.");
    mu_assert(List_first(a) == test1, "Wrong first item of first list after second split.");
    mu_assert(List_last(a) == test2, "Wrong last item of first list after second split.");
    mu_assert(List_first(b) == test3, "Wrong first item of second list after second split.");
    mu_assert(List_last(b) == test3, "Wrong last item of second list after second split.");

    List_destroy(a);
    List_destroy(b);

    return NULL;
}

char *all_tests() {
    mu_suite_start();

    mu_run_test(test_create);
    mu_run_test(test_push_pop);
    mu_run_test(test_unshift);
    mu_run_test(test_copy);
    mu_run_test(test_join);
    mu_run_test(test_split);
    mu_run_test(test_remove);
    mu_run_test(test_shift);
    mu_run_test(test_destroy);

    return NULL;
}

RUN_TEST(all_tests);
