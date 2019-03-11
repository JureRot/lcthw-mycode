#ifndef lcthw_List_algos_h
#define lcthw_List_algos_h

#include <lcthw/list.h>

//define a function pointer to which we will pass comparison functions (such as strcmp)
//the function it points to takes two void pointers (so those could be poiters of any type) and returns int (like compare functuons usually do (-x for first smaller, 0 for eaqual, +x for first bigger))
typedef int (*List_compare) (const void *a, const void *b);

int List_bubble_sort(List *list, List_compare cmp);

List *List_merge_sort(List *list, List_compare cmp);

#endif
