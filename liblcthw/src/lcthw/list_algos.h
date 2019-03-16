#ifndef lcthw_List_algos_h
#define lcthw_List_algos_h

#include <lcthw/list.h>

//define a function pointer to which we will pass comparison functions (such as strcmp)
//the function it points to takes two void pointers (so those could be poiters of any type) and returns int (like compare functuons usually do (-x for first smaller, 0 for eaqual, +x for first bigger))
typedef int (*List_compare) (const void *a, const void *b);

//gets a list and a comparation function and sorts the list using bubble sort
int List_bubble_sort(List *list, List_compare cmp);

//gets a list and a comparation function and returns a new lists sorted using merge sort
List *List_merge_sort(List *list, List_compare cmp);

//gets a sorted list and a value and using comparation function it inserts it so, that the list is still sorted (inserts in front if equal)
void List_insert_sorted(List* list, void *value, List_compare cmp);

//gets a list and a compariation function and returns a new list sorted uising bottom up merge sort
List *List_merge_bottom_up(List *list, List_compare cmp);

#endif
