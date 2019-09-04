#ifndef _lcthw_Hashmap_h
#define _lcthw_Hashmap_h

#include <stdint.h>
#include <lcthw/darray.h>

//define defualt number of buckets in hash (could be dynamic or user defined)
//#define DEFAULT_NUMBER_OF_BUCKETS 100
//for testing purposes
#define DEFAULT_NUMBER_OF_BUCKETS 3

//function pointer to a comparison function
typedef int (*Hashmap_compare) (void *a, void *b);
//function pointer to a hashing function
typedef uint32_t (*Hashmap_hash) (void *key);

//structure of the Hashmap
//buckets is a dynamic array (DArray) of (in our case 100) buckets,
//    each of which is a dynamic array of HashmapNodes
//compare is a pointer to a comparison function
//hash is a pointer to a hashing function
typedef struct Hashmap {
    DArray *buckets;
    Hashmap_compare compare;
    Hashmap_hash hash;
} Hashmap;

//structure of HashmapNode
//key is a void pointer to the key value
//key is a void pointer to the data value
//hash is uint32_t value of hash of key
typedef struct HashmapNode {
    void *key;
    void *data;
    uint32_t hash;
} HashmapNode;

//function pointer to a traverse function of some sort (look in hashmap_tests.c)
typedef int (*Hashmap_traverse_cb) (HashmapNode *node);

//these are all declarations of functions (nothing special)
Hashmap *Hashmap_create(Hashmap_compare compare, Hashmap_hash hash);
void Hashmap_destroy(Hashmap *map);

int Hashmap_set(Hashmap *map, void *key, void *data);
void *Hashmap_get(Hashmap *map, void *key);

int Hashmap_traverse(Hashmap *map, Hashmap_traverse_cb traverse_cb);

void *Hashmap_delete(Hashmap *map, void *key);

#endif
