#include <lcthw/list_algos.h>
#include "list_algos.h" //just for writing
#include "list.h"
#include <lcthw/dbg.h>
#include "dbg.h" //just for writing

//a and b are sequential nodes in list
//spaws the order of a and b
//example: wxabyz -> wxbayz
//TODO
void swap_nodes(List *list, ListNode *a, ListNode *b) {
    printf("%s %s\n", a->value, b->value);
    //if a is first element in list
    //if b is last element in list
    //if a and b are somewhere in the middle
}

int List_bubble_sort(List *list, List_compare cmp) {
    check(list, "Can't sort NULL list using compare function.");
    check(cmp, "Can't sort list using NULL compare function.");

    //the problem is how to use LIST_FOREACH here

    return 0;

error:
    return NULL; //dont know if i can return NULL or should i exit(1)
}

List *List_merge_sort(List* list, List_compare cmp) {
    check(list, "Can't sort NULL list using compare function.");
    check(cmp, "Can't sort list using NULL compare function.");

    return list;

error:
    return NULL; //dont know if i can return NULL or should i exit(1)
}
