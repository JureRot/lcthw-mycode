#include <lcthw/list.h>
//#include "list.h"
#include <lcthw/dbg.h>
//#include "dbg.h"

List *List_create() {
    return calloc(1, sizeof(List));
}

void List_destroy(List *list) {
    check(list, "Can't destroy NULL list.");

    //invariant check
    int cnt = list->count;
    if (cnt < 0) {
        log_err("Count of list is less than zero.");
        goto error;
    }

    LIST_FOREACH(list, first, cur, next) { //i dont understand how LIST_FOREACH knows what is first, next and cur
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
    int cnt = list->count;
    if (cnt < 0) {
        log_err("Count of list is less than zero.");
        goto error;
    }

    LIST_FOREACH(list, first, cur, next) {
        free(cur->value);
    }

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
    int cnt = list->count;
    if (cnt < 0) {
        log_err("Count of list is less than zero.");
        goto error;
    }

    LIST_FOREACH(list, first, cur, next) {
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
    int cnt = list->count;
    if (cnt < 0) {
        log_err("Count of list is less than zero.");
        goto error;
    }

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

error:
    return;
}

void *List_pop(List *list) { //pops the last element (from the end)
    check(list, "Can't pop from NULL list.");

    //invariant check
    int cnt = list->count;
    if (cnt < 0) {
        log_err("Count of list is less than zero.");
        goto error;
    }

    ListNode *node = list->last;
    return node != NULL ? List_remove(list, node) : NULL;

error:
    return NULL;
}

void List_unshift(List *list, void *value) { //push to the front
    check(list, "Can't unshift value to NULL list.");
    check(value, "Can't unshift NULL value to list.");

    //invariant check
    int cnt = list->count;
    if (cnt < 0) {
        log_err("Count of list is less than zero.");
        goto error;
    }

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

error:
    return;
}

void *List_shift(List *list) { //pop from the front
    check(list, "Can't shift from NULL list.");

    //invariant check
    int cnt = list->count;
    if (cnt < 0) {
        log_err("Count of list is less than zero.");
        goto error;
    }

    ListNode *node = list->first;
    return node != NULL ? List_remove(list, node) : NULL;

error:
    return NULL;
}

void *List_remove(List *list, ListNode *node) {
    void *result = NULL;

    check(list, "Can't remove node from NULL list.");

    //invariant check
    int cnt = list->count;
    if (cnt < 0) {
        log_err("Count of list is less than zero.");
        goto error;
    }

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
        check(list->last != NULL, "Invalid list, somehow got a next that is NULL.");
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

error:
    return result;
}
