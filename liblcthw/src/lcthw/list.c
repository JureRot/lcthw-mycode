#include <lcthw/list.h>
//#include "list.h"
#include <lcthw/dbg.h>
//#include "dbg.h"

int cnt = 0;

List *List_create() {
    return calloc(1, sizeof(List));
}

void List_destroy(List *list) {
    check(list, "Can't destroy NULL list.");

    //invariant check
    cnt = list->count;
    check((cnt==0) || (cnt>0 && (list->first && list->last)), "List either has count less than zero or has count greater than zero, but doesn't have first or last element.");

    LIST_FOREACH(list, first, next, cur) { //i dont understand how LIST_FOREACH knows what is first, next and cur
        if (cur->prev) {
            free(cur->prev);
        }
    }

    free(list->last);
    free(list);

error:
    return;
}

void List_clear(List *list) {
    check(list, "Can't clear NULL list.");

    //invariant check
    cnt = list->count;
    check((cnt==0) || (cnt>0 && (list->first && list->last)), "List either has count less than zero or has count greater than zero, but doesn't have first or last element.");

    LIST_FOREACH(list, first, next, cur) {
        free(cur->value);
    }

    //invariant check
    cnt = list->count;
    check((cnt==0) || (cnt>0 && (list->first && list->last)), "List either has count less than zero or has count greater than zero, but doesn't have first or last element.");

error:
    return;
}

/*
void List_clear_destroy(List *list) { //slow (loop over list twice)
    List_clear(list);
    List_destroy(list);
}
*/


void List_clear_destroy(List *list) { //fast (only one loop)
    check(list, "Can't clear and destroy NULL list.");

    //invariant check
    cnt = list->count;
    check((cnt==0) || (cnt>0 && (list->first && list->last)), "List either has count less than zero or has count greater than zero, but doesn't have first or last element.");

    LIST_FOREACH(list, first, next, cur) {
        free(cur->value);
        if (cur->prev) {
            free(cur->prev);
        }
    }

    free(list->last);
    free(list);

error:
    return;
}

void List_push(List *list, void *value) { //adds to the end
    check(list, "Can't push value to NULL list.");
    check(value, "Can't push NULL value to list.")

    //invariant check
    cnt = list->count;
    check((cnt==0) || (cnt>0 && (list->first && list->last)), "List either has count less than zero or has count greater than zero, but doesn't have first or last element.");

    ListNode *node = calloc(1, sizeof(ListNode));
    check_mem(node);

    node->value = value;

    if (list->last == NULL) {
        list->first = node;
        list->last = node;
    } else {
        list->last->next = node;
        node->prev = list->last;
        list->last = node;
    }

    list->count++;

    //invariant check
    cnt = list->count;
    check((cnt==0) || (cnt>0 && (list->first && list->last)), "List either has count less than zero or has count greater than zero, but doesn't have first or last element.");

error:
    return;
}

void *List_pop(List *list) { //pops the last element (from the end)
    check(list, "Can't pop from NULL list.");

    //invariant check
    cnt = list->count;
    check((cnt==0) || (cnt>0 && (list->first && list->last)), "List either has count less than zero or has count greater than zero, but doesn't have first or last element.");

    ListNode *node = list->last;
    return node != NULL ? List_remove(list, node) : NULL;

error:
    return NULL;
}

void List_unshift(List *list, void *value) { //push to the front
    check(list, "Can't unshift value to NULL list.");
    check(value, "Can't unshift NULL value to list.");

    //invariant check
    cnt = list->count;
    check((cnt==0) || (cnt>0 && (list->first && list->last)), "List either has count less than zero or has count greater than zero, but doesn't have first or last element.");

    ListNode *node = calloc(1, sizeof(ListNode));
    check_mem(node);

    node->value = value;

    if (list->first == NULL) {
        list->first = node;
        list->last = node;
    } else {
        node->next = list->first;
        list->first->prev = node;
        list->first = node;
    }

    list->count++;

    //invariant check
    cnt = list->count;
    check((cnt==0) || (cnt>0 && (list->first && list->last)), "List either has count less than zero or has count greater than zero, but doesn't have first or last element.");

error:
    return;
}

void *List_shift(List *list) { //pop from the front
    check(list, "Can't shift from NULL list.");

    //invariant check
    cnt = list->count;
    check((cnt==0) || (cnt>0 && (list->first && list->last)), "List either has count less than zero or has count greater than zero, but doesn't have first or last element.");

    ListNode *node = list->first;
    return node != NULL ? List_remove(list, node) : NULL;

error:
    return NULL;
}

void *List_remove(List *list, ListNode *node) {
    void *result = NULL;

    check(list, "Can't remove node from NULL list.");

    //invariant check
    cnt = list->count;
    check((cnt==0) || (cnt>0 && (list->first && list->last)), "List either has count less than zero or has count greater than zero, but doesn't have first or last element.");

    check(list->first && list->last, "List is empty.");
    check(node, "node can't be NULL.");

    if (node == list->first && node == list->last) { //if list only one element
        list->first = NULL;
        list->last = NULL;
    } else if (node == list->first) { //if removing node from front
        list->first = node->next;
        check(list->first != NULL, "Invalid list, somehow got a first that is NULL.");
        list->first->prev = NULL;
    } else if (node == list->last) { //if removing node from tail
        list->last = node->prev;
        check(list->last != NULL, "Invalid list, somehow got a last that is NULL.");
        list->last->next = NULL;
    } else { //if node to be removed is in the middle
        ListNode *after = node->next;
        ListNode *before = node->prev;
        after->prev = before;
        before->next = after;
    }

    list->count--;
    result = node->value;
    free(node);

    //invariant check
    cnt = list->count;
    check((cnt==0) || (cnt>0 && (list->first && list->last)), "List either has count less than zero or has count greater than zero, but doesn't have first or last element.");

error: //fallthrough
    return result;
}

List *List_copy(List *list) {
    List *result = NULL;

    check(list, "Can't copy NULL list.");

    //invariant check
    cnt = list->count;
    check((cnt==0) || (cnt>0 && (list->first && list->last)), "List either has count less than zero or has count greater than zero, but doesn't have first or last element.");

    result = calloc(1, sizeof(List));
    check_mem(result);

    LIST_FOREACH(list, first, next, cur) {
        List_push(result, cur->value);
    }

error: //fallthrough
    return result;

}

List *List_join(List *list1, List *list2) {
    List *result = NULL;

    check(list1, "Can't join NULL list with list.");
    check(list2, "Can't join list with NULL list.");

    //invariant check
    cnt = list1->count;
    check((cnt==0) || (cnt>0 && (list1->first && list1->last)), "First list either has count less than zero or has count greater than zero, but doesn't have first or last element.");
    cnt = list2->count;
    check((cnt==0) || (cnt>0 && (list2->first && list2->last)), "Second list either has count less than zero or has count greater than zero, but doesn't have first or last element.");

    list1->last->next = list2->first;
    list2->first->prev = list1->last;

    list1->last = list2->last;

    list1->count += list2->count;

    result = list1;
    free(list2);

error: //fallthrough
    return result;
}

List *List_split(List *list, ListNode *node) {
    List *result = NULL;

    check(list, "Can't split NULL list at node.");
    check(node, "Can't split list at NULL node.");
    check(List_count(list) > 1, "Can't split list with less than two elements..");
    check(node->prev, "Can't split on first element (would produce empty list).");

    //invariant check
    cnt = list->count;
    check((cnt==0) || (cnt>0 && (list->first && list->last)), "List either has count less than zero or has count greater than zero, but doesn't have first or last element.");

    result = calloc(1, sizeof(List));

    int new_count = List_count(list);

    LIST_FOREACH(list, first, next, cur) { //need to go over them to get the new count values
        if (cur == node) {
            break;
        }
        new_count--;
    }

    result->first = node; //set the first and last nodes of new (second) list
    result->last = list->last;
    list->last = node->prev; //set the end of original (first) list to just before the node

    node->prev->next = NULL; //remove connections between the two lists
    node->prev = NULL;

    list->count -= new_count; //update the counts for both lists
    result->count = new_count;
    //this part of the code is without checks because we do all the neccessary checks at the beginning of the function.

error: //fallthrough
    return result;
}
