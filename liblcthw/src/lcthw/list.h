#ifndef lcthw_List_h
#define lcthw_List_h

#include <stdlib.h>

//struct of a node in double linked list
//holds two pointers of type ListNode for next and previous node in list
//and pointer of type void for value
struct ListNode;
typedef struct ListNode {
    struct ListNode *prev;
    struct ListNode *next;
    void *value;
} ListNode;

//struct of list
//hold integer of number of nodes
//and two pointers of type ListNode that point to the first and last element in list (or are NULL if list is empty)
typedef struct List {
    int count;
    ListNode *first;
    ListNode *last;
} List;

//callocs enough space for List struct
List *List_create();
//frees all nodes in list and frees the list as well
void List_destroy(List *list);
//frees the values of all nodes in list
void List_clear(List *list);
//frees the values of all nodes and frees all nodes in list and frees the list as well
void List_clear_destroy(List *list);

//returns count of the list
#define List_count(A) ((A)->count)
//returns first element of the list or NULL if there is none
#define List_first(A) ((A)->first != NULL ? (A)->first->value : NULL)
//returns last element of the list of NULL if there is none
#define List_last(A) ((A)->last != NULL ? (A)->last->value : NULL)

//adds node with value value to the end of the list
void List_push(List *list, void *value);
//removes and returns the value of the node at the end of the list
void *List_pop(List *list);

//adds node with value value to the beginning of the list
void List_unshift(List *list, void *value);
//removes and returns the value of the node at the beginning of the list
void *List_shift(List *list);

//removes node from list
void *List_remove(List *list, ListNode *node);

//creates a new list as a copy of the original one
List *List_copy(List *list);

//joins two lists
//returns the pointer to the beginning of the first one, the pointer to the second one is removed (freed)
List *List_join(List *list1, List *list2);

//splits two lists at the node (node becomes the first element in second list)
//returs the pointer to the beginning of the second list
//the first (original) list still exists, it's just shortened
List *List_split(List *list, ListNode *node);

//defines the macro for loopin over the whole list
#define LIST_FOREACH(L, S, M, V) ListNode *_node = NULL;\
                                                   ListNode *V = NULL;\
for(V = _node = L->S; _node != NULL; V = _node = _node->M)

#endif
