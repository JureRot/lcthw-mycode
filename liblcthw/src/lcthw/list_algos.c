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

//gets two already sorted arrays and joins them into one sorted aray using cmp to compare values
List *join_sorted(List *a, List *b, List_compare cmp) {
    check (a && b, "Can't join NULL sorted lists using compare function.");
    check (cmp, "Can't join sorted lists using NULL compare function.");

    List *result = List_create();
    check_mem(result);

    while (List_count(a)>0 && List_count(b)>0) {
        if (cmp(List_first(a), List_first(b)) > 0) { //if a bigger than b
            List_push(result, List_shift(b)); //remove the first element of b and push it to the end of result
        } else { //if a smaller or eaqual than b
            List_push(result, List_shift(a)); //we remove the first element of a and push to to the end of result
        }
    }


    if (List_count(a) == 0) { //if a ran out of elements we put all the rest of b to result
        List_destroy(a); //we delete a, b is deleted in List_join()
        result = List_join(result, b);
    } else { //b ran out of elements
        List_destroy(b);
        result = List_join(result, a);
    }

    //idea:
    //creat a new list (result)
    //sequentially push to result the smallest firsts from a and b
    //when a or b count becomes 0, destroy that list and join the other to the result

    return result;

error:
    return NULL;
}

//returns the node in the middle (if even, returns the one after the middle point)
ListNode *get_middle_node(List *list) {
    ListNode *result = NULL;

    check(list, "Can't get middle of NULL list.");

    int half = List_count(list) / 2; //because count is in, this is whole number division

    int i = 0;
    LIST_FOREACH(list, first, next, cur) {
        if (i++ == half) {
            result = cur;
            break;
        }
    }
    //there should be a get_node function in default list (maybe add)

error: //fallthrough
    return result;
}

List *List_merge_sort(List* list, List_compare cmp) {
    check(list, "Can't sort NULL list using compare function.");
    check(cmp, "Can't sort list using NULL compare function.");

    if (List_count(list) < 2) {
        return list;
    }

    List *split_list = List_split(list, get_middle_node(list)); //we split list at the middle node

    list = List_merge_sort(list, cmp); //recursively sort each half
    split_list = List_merge_sort(split_list, cmp);

    return join_sorted(list, split_list, cmp); //merge them back together (still sorted)

error:
    return NULL; //dont know if i can return NULL or should i exit(1)

    //there are two approaches to merge sort:
    //top-down (implemented here) where we recursively split list into sublists of len 1 and than we join/merge them toghther in a sorted way
    //bottom-up: we treat the list like individual nodes of len 1 and we recursively sort exerpts/sections of that list (first len 2, than 4, 8, ...)
    //
    //the top-down approach seems more logical to me and uses shift and push functions (already implemented)
    //on the other hand bottom-up approach needs to implement insert which is hard in double linked list and is more confusing to understand

    //idea:
    //we use recursion to split the list in the middle into two lists until each of the lists is only one element long
    //we can merge the list back together in such a way that its still sorted
    //and so we backtrace all the way to the beginning of recursion, mergin sorted lists into bigger still stored lists
    //until we arive back at the start where we merge first half of the original list (now sorted) to the other (also sorted) half of the original list and get a full sorted list
    //(this recursion is DFS)
}
