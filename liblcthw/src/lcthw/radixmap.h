#ifndef _radixmap_h
#define _radixmap_h //this line is not in source

#include <stdint.h>
#include <stdlib.h> //size_t is in stdlib

//union of RedixMap element
//a bit unusual because we will use black magic to work with it
typedef union RMElement {
    uint64_t raw;
    struct {
        uint32_t key;
        uint32_t value;
    } data;
} RMElement;
//we exploit union here by making key and value an actual part of raw
//because its an union they overlap so raw is made out of key and value one next to the other

//structure of radix map
//has max number of elements max
//has current number of elements end
//has counter
//has contents and temp pointers (pointing to the firs elemen in array of elements)
typedef struct RadixMap {
    size_t max;
    size_t end;
    uint32_t counter;
    RMElement *contents;
    RMElement *temp;
} RadixMap;

//creates a new RadixMap with max number of elements
//returns pointer to that map
RadixMap *RadixMap_create(size_t max);

//destroys RadixMap (contents, temp and map itself)
void RadixMap_destroy(RadixMap *map);

//sorts the RedixMap
void RadixMap_sort(RadixMap *map);

//finds element in RadixMap by key
//returns pointer to that element
RMElement *RadixMap_find(RadixMap *map, uint32_t key);

//adds new elements with key and value to the array
//makes sure the array stays sorted
//returns 0 on success, -1 on error
int RadixMap_add(RadixMap *map, uint32_t key, uint32_t value);

//removes element from array, keeping it sorted
//returns 0 on success, -1 on error
int RadixMap_delete(RadixMap *map, RMElement *el);

#endif
