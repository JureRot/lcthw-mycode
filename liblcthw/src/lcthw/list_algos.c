#include <lcthw/list_algos.h>
#include "list_algos.h" //just for writing
#include "list.h"
#include <lcthw/dbg.h>
#include "dbg.h" //just for writing

//a and b are sequential nodes in list
//spaws the order of a and b
//example: wxabyz -> wxbayz
void swap_nodes(List *list, ListNode *a, ListNode *b) {
    check(list, "Can't swap nodes in NULL list.");
    check(a && b, "Can't swap NULL nodes in list.");
    check(a->next == b, "b does not follow a.");

    if (list->first == a && list->first == b) { //list only 2 elements
        a->next = NULL;
        a->prev = b;
        b->next = a;
        b->prev = NULL;
        list->first = b;
        list->last = a;

    } else if (list->first == a) { //a is first element in list
        b->next->prev = a;
        a->next = b->next;
        a->prev = b;
        b->next = a;
        b->prev = NULL;
        list->first = b;

    } else if (list->last == b) { //b is last element in list
        a->prev->next = b;
        b->next = a;
        b->prev = a->prev;
        a->next = NULL;
        a->prev = b;
        list->last = a;

    } else { //a and b are somewhere in between
        a->prev->next = b;
        b->next->prev = a;
        a->next = b->next;
        b->prev = a->prev;
        a->prev = b;
        b->next = a;

    }

error:
    return;
}

int List_bubble_sort(List *list, List_compare cmp) {
    check(list, "Can't sort NULL list using compare function.");
    check(cmp, "Can't sort list using NULL compare function.");

    int n = List_count(list);
    int new_n = 0;

    while (n > 1) {
        ListNode *cur = list->first;
        new_n = 0;

        for (int i=0; i<n-1; i++) {
            check(cur && cur->next, "Count of list does not match the actual list.");
            check(cur->value && cur->next->value, "Can't compare/sort NULL values.");

            if (cmp(cur->value, cur->next->value) > 0) {
                swap_nodes(list, cur, cur->next);
                cur = cur->prev; //go one back to we dont skip over one element after swap
                new_n = i+1; //remember the location of the last swap
            }
            cur = cur->next;
        }
        n = new_n;
    }
    //we cant use LIST_FOREACH because we need to change cur but we cant because there is a hidden _node var that actually effects cur

    //idea:
    //for list of len n
    //we go from beggining to the end of the list n times
    //every step of iteration we compare the two elements and put the bigger one towards the end
    //(efectively, we go from beginning to the end n-times and every time we take the biggest element with us)
    //
    //optimization:
    //after the fist iteration over list, the biggest element is at the very end and won't ever move again
    //so in the next iteration we dont need to go to the very end, but one less
    //even more:
    //maybe more than just the biggest element is in its right position
    //thats why we keep note where the last swap in interation was
    //all the elements after that swap are already sorted and won't ever move
    //so the next iteration will go only until the location of the last swap in previous iteration

    return 0;

error:
    return NULL; //dont know if i can return NULL or should i exit(1)
}

//TODO
List *List_merge_sort(List* list, List_compare cmp) {
    check(list, "Can't sort NULL list using compare function.");
    check(cmp, "Can't sort list using NULL compare function.");

    //its easy to split lists using List_split()
    //but List_join() does not work correcly for this application (will need to use List_shift sequentially)

    //also there are two approacehs
    //top-down and bottom-up

    return list;

error:
    return NULL; //dont know if i can return NULL or should i exit(1)
}
