#undef NDEBUG
#include <stdint.h>
#include <lcthw/hashmap.h>
//#include "hashmap.h" //just for writting
#include <lcthw/dbg.h>
//#include "dbg.h" //just for writting
#include <lcthw/bstrlib.h>
//#include "bstrlib.h" //just for writting

static int default_compare(void *a, void *b) {
    return bstrcmp((bstring) a, (bstring) b);
}

int hashnodecmp(HashmapNode *a, HashmapNode *b) {
	//I DONT KNOW WHAT IS PASSED TO THIS
    return bstrcmp((bstring)a->key, (bstring)b->key);
}

/*
 * Simple Bob Jenkin's hash algorithm taken from the wikipedia description.
 */

//some sort of simple hashing funciton using shifting, addint and bitwise xoring
static uint32_t default_hash(void *a) {
    size_t len = blength((bstring) a);
    char *key = bdata((bstring) a);
    uint32_t hash = 0;
    uint32_t i = 0;

    for (hash=i=0; i<len; ++i) {
        hash += key[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }

    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);

	//for testing
	//printf("value: %s, len: %d, hash: %d, bucket_n: %d\n", key, len, hash, hash%100);

    return hash;
}

//create function
//takes compare function pointer and hashing functin pointer
//returns pointer to Hashmap
Hashmap *Hashmap_create(Hashmap_compare compare, Hashmap_hash hash) {
    Hashmap *map = calloc(1, sizeof(Hashmap)); //we reserve space
    check_mem(map);

    map->compare = compare==NULL ? default_compare : compare; //use ternary to assign compare and hash functs
    map->hash = hash==NULL ? default_hash : hash;
    map->buckets = DArray_create(sizeof(DArray *), DEFAULT_NUMBER_OF_BUCKETS); //reserve 100 buckets (which are darrays in it self)
    map->buckets->end = map->buckets->max; // fake out expanding it
    //i think this makes it look as if the buckets are full (end is set to max)
    check_mem(map->buckets);

    return map;

error:
    if (map) {
        Hashmap_destroy(map);
    }

    return NULL;
}

//destroy function
//takes pointer to map
//returns nothing
void Hashmap_destroy(Hashmap *map) {
    //int i = 0;
    //int j = 0;

    if (map) { //frees map
        if (map->buckets) { //and if map has buckets,
            for (int i=0; i<DArray_count(map->buckets); i++) { //frees all buckets
                DArray *bucket = DArray_get(map->buckets, i);
                if (bucket) { //if they exist
                    for (int j=0; j<DArray_count(bucket); j++) {
                        free(DArray_get(bucket, j));
                    }
                    DArray_destroy(bucket);
                }
            }
            DArray_destroy(map->buckets);
        }
        free(map);
    }
}

//inline funct to create HashmapNodes
//takes hash, pointer to key and pointer to value
//returns pointer to hashmap node
static inline HashmapNode *Hashmap_node_create(int hash, void *key, void *data) {
    HashmapNode *node = calloc(1, sizeof(HashmapNode)); //reserves space for node
    check_mem(node);

    //sets the values
    node->key = key;
    node->data = data;
    node->hash = hash;

    return node;

error:
    return NULL;
}

//funct to find the bucket in map
//takes pointer to map, pointer to key, int (flag) create and pointer to uint32_t that we can change so the caller can use it and get the hash of bucket
//returns pointer th the bucket
static inline DArray *Hashmap_find_bucket(Hashmap *map, void *key, int create, uint32_t *hash_out) {
    uint32_t hash = map->hash(key); //we use the function pointer in map as a function to get the hash of the key (VERY INTERESTING)
    int bucket_n = hash % DEFAULT_NUMBER_OF_BUCKETS; //we use modulo to determine in which bucket we should look
    check(bucket_n >= 0, "Invalid bucket found: %d", bucket_n); //check if bucket number is valid
    // store it for the return so the caller can use it
    *hash_out = hash;

    DArray *bucket = DArray_get(map->buckets, bucket_n); //we get the bucket

    if (!bucket && create) { //if we cant get the bucket and the create flag is set, we create the bucket
        // new bucket, set it up
        //DONT KNOW WHY THERE IS DEFAULT NUMBER HERE AND VOID*, WE NEED TO CREATE JUST ONE BUCKET
        //bucket = DArray_create(sizeof(void *), DEFAULT_NUMBER_OF_BUCKETS);
        //bucket = DArray_create(sizeof(DArray *), 1); // i think this is more correct (we just create one bucket of size DArray)
        bucket = DArray_create(sizeof(HashmapNode *), 1); // i think this is even more correct (we just create one bucket of size HashmapNode) (or void* ???)
		//the moment we push to this bucket(DArray) containing only one HashmapNode it will expand to 300 elements
		//if we had DArray_expand implemented correctly (size times two for instance) it would make a lot more sense (but even now it works and it is correct)
		//(we could make the staring size of 2 to dalay the expansion until two elements have the same hash)

		//TODO
		//this still may not be correct and working for all edge-cases
		//some testing is still recommended

        check_mem(bucket);
        DArray_set(map->buckets, bucket_n, bucket); //we set the newly created bucket into buckets
    }

    return bucket;

error:
    return NULL;
}

//set function
//takes pointer to map, pointer to key and pointer to data
//returns int
int Hashmap_set(Hashmap *map, void *key, void *data) {
    uint32_t hash = 0;
    DArray *bucket = Hashmap_find_bucket(map, key, 1, &hash); //we get the pointer to the bucket in which we should set the node (based on key)
    //hash is also set (look Hashmap_find_bucket), so we have hash of the key
    check(bucket, "Error, can't create bucket.");

    HashmapNode *node = Hashmap_node_create(hash, key, data); //we create the node
    check_mem(node);

    DArray_push(bucket, node); //we push the node to the correct bucket

    //here we could sort the bucket to have he nodes inside buckets sorted (easier search and set)
	//cant we just use DArray_qsort or DArray_sort_add for this (already implemented in darray_algos.c)
	//DONT THINK SO, BECAUSE WE WORK WITH BSTRINGS HERE (need to compare HashmapNodes, not chars in array)

	//this is an improvement
	//we sort the bucket after insert (could have done a sorted insert (DArray_sort_add)) (slower insert but faster find)
	DArray_qsort(bucket, hashnodecmp);
	//DArray_my_qsort(bucket, hashnodecmp);

    return 0;

error:
    return -1;
}

//inline funct to get the node
//takes pointer to map, value of hash, pointer to bucket and pointer to key
//returns int (which is the index of the node in bucket)
static inline int Hashmap_get_node(Hashmap *map, uint32_t hash, DArray *bucket, void *key) {
    //int i = 0;

    for (int i=0; i<DArray_end(bucket); i++) { //we go over all then nodes in the bucket
        debug("TRY: %d", i);
        HashmapNode *node = DArray_get(bucket, i);
        if (node->hash == hash && map->compare(node->key, key) == 0) { //if hash and keys match, we return index
            return i;
        }
    }

    //if the buckets were sorted we could use binary search to find the node faster
	//cant we just use DArray_find for this (already implemented in darray_algos.c)
	//DONT THINK SO, BECAUSE WE WORK WITH BSTRINGS HERE (need to compare HashmapNodes, not chars in array)
	//why doesn't DArray_find work here (something is wrong)

    return -1;
}

static inline int Hashmap_get_node_binary_search(Hashmap *map, uint32_t hash, DArray *bucket, void *key) {
	//This is a copy of the one in the darray_algos
	//I tried to make that one work with this but had not success

	HashmapNode *el = Hashmap_node_create(hash ,key, NULL);
	//HashmapNode *n = DArray_get(bucket, 3);
	//int cmp = hashnodecmp(el, n);

	//int f = DArray_find(bucket, el, hashnodecmp);
	//printf("cmp: %d, f: %d\n", cmp, f);

    int low = 0;
    int high = bucket->end - 1;
    //int mid = -1;

    while (low <= high) {
        int mid = low + (high - low) / 2;
        int compare = hashnodecmp(el, bucket->contents[mid]);

        if (compare == 0) { //el is equal to middle, so we return indes of middle
			free(el);
            return mid;
        } else if (compare < 0) { //el smaller than current middle, go to lower half
            high = mid - 1;
        } else if (compare > 0) { //el bigger than current middle, go to highter half
            low = mid + 1;
        }
    }

	free(el);

error: //fallthrough
    return -1;
}

//funct to get value of key in hashmap
//takes pointer to map and pointer to key
//returns void pointer (to data of node)
void *Hashmap_get(Hashmap *map, void *key) {
    uint32_t hash = 0;
    DArray *bucket = Hashmap_find_bucket(map, key, 0, &hash); //we get the pointer to bucket of key and the hash (thanks to Hashmap_find_bucket function)
    if (!bucket) return NULL; //if bucket not found, we (we didnt create it), we just return null

    //int i = Hashmap_get_node(map, hash, bucket, key); //we get the index of node inside the bucket
    int i = Hashmap_get_node_binary_search(map, hash, bucket, key); //my binary search version
    if (i == -1) return NULL; //if wrong index, we return null

    HashmapNode *node = DArray_get(bucket, i); //get the to node at the spedific index in the specific bucket
    check(node != NULL, "Failed to get node from bucket when it should exist.");

    return node->data; //we return the value of data

error: // fallthrough
    return NULL;
}

//useful traverse function (to be called on every node or such)
//takes pointer to map and function pointer to traverse_cb function
//returns int
int Hashmap_traverse(Hashmap *map, Hashmap_traverse_cb traverse_cb) {
    //int i = 0;
    //int j = 0;
    int rc = 0;

    //for every node in every bucket we call traverse_cb on node
    //traverse_cb is a funct pointer that takes pointer to node an returns intr (0 on success, other on error)
    for (int i=0; i<DArray_count(map->buckets); i++) {
        DArray *bucket = DArray_get(map->buckets, i);
        if (bucket) {
            for (int j=0; j<DArray_count(bucket); j++) {
                HashmapNode *node = DArray_get(bucket, j);
                rc = traverse_cb(node);
                if (rc != 0)
                    return rc;
            }
        }
    }

    return 0;
}

//delete function
//takes pointer to map and pointer to key
//returns void pointer (to value of data of deleted node (like pop))
void *Hashmap_delete(Hashmap *map, void *key) {
    uint32_t hash = 0;
    DArray *bucket = Hashmap_find_bucket(map, key, 0, &hash); //get pointer to bucket and hash of key
    if (!bucket)
        return NULL;

    //int i = Hashmap_get_node(map, hash, bucket, key); //get index of node in bucket
    int i = Hashmap_get_node_binary_search(map, hash, bucket, key); //my binary search version (DONT KNOW IF IT WORKS)
    if (i == -1)
        return NULL;

    HashmapNode *node = DArray_get(bucket, i); //get node
    void *data = node->data; //remember the data
    free(node); //we free the node

    HashmapNode *ending = DArray_pop(bucket); //and we pop the last node in bucket (not necessarily the one we need)

    if (ending != node) { //if it wasnt the last node in the bucket
        // alright, looks like it's not the last one, swap it
        DArray_set(bucket, i, ending); //we set the value of our node to the value of the one we poped (the last)
    }

    return data;
}
