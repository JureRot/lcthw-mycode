/*
 * Based on code by Andre Reinald then heavily modified by Zed A. Shaw.
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <lcthw/radixmap.h>
//#include "radixmap.h" //just for writting
#include <lcthw/dbg.h>
//#include "dbg.h" //just for writting

RadixMap *RadixMap_create(size_t max) {
    RadixMap *map = calloc(1, sizeof(RadixMap)); //allocs size for RadixMap struct
    check_mem(map);

    map->contents = calloc(max+1, sizeof(RMElement)); //allocs block of memory for max+1 RMElements (will act as an array in how we will access it)
    check_mem(map->contents);

    map->temp = calloc(max+1, sizeof(RMElement)); //allocs another block of memory for temp
    check_mem(map->temp);

    map->max = max; //sets max and end
    map->end = 0;

    return map;

error:
    RadixMap_destroy(map); //this line is not in source
    return NULL;
}

void RadixMap_destroy(RadixMap *map) {
    if (map) { //if map exits, free contents, temp and the map itself
        free(map->contents);
        free(map->temp);
        free(map);
    }
}

#define ByteOf(x, y) (((uint8_t *)x)[(y)])
//macro for getting sepecific byte of larger numbers (such as uint32 and uint64)
//smaller y is the more important (lefter byte) of the number

//inline may NOT work on osx
static inline void radix_sort(short offset, uint64_t max, uint64_t *source, uint64_t *dest) {
    uint64_t count[256] = { 0 }; //create an array of 256 ints (one for every number in a byte)
    //instead of 10 values in domain for radix sort we will use 256
    uint64_t *cp = NULL; //and init some helping vars
    uint64_t *sp = NULL;
    uint64_t *end = NULL;
    uint64_t s = 0;
    uint64_t c = 0;
    //offset is telling us which 8 bits we are looking at (look at how radix sort works)

    // count occurneces of every byte value
    for (sp=source, end=source+max; sp<end; sp++) {
        count[ByteOf(sp, offset)]++;
    }

    // tranform count into index by summing elements and storing into same array
    for (s=0, cp=count, end=count+256; cp<end; cp++) {
        c = *cp;
        *cp = s;
        s += c;
    }

    // fill dest with the right values in the right place
    for (sp=source, end=source+max; sp<end; sp++) {
        cp = count + ByteOf(sp, offset);
        dest[*cp] = *sp;
        ++(*cp);
    }
}

void RadixMap_sort(RadixMap *map) {
    uint64_t *source = &map->contents[0].raw;
    uint64_t *temp = &map->temp[0].raw;

    radix_sort(0, map->end, source, temp);
    radix_sort(1, map->end, temp, source);
    radix_sort(2, map->end, source, temp);
    radix_sort(3, map->end, temp, source);
}

RMElement *RadixMap_find(RadixMap *map, uint32_t to_find) { //binary search
    int low = 0;
    int high = map->end - 1;
    RMElement *data = map->contents;

    while (low <= high) {
        int middle = low + (high - low) / 2;
        uint32_t key = data[middle].data.key;

        if (to_find < key) {
            high = middle - 1;
        } else if (to_find > key) {
            low = middle + 1;
        } else {
            return &data[middle];
        }
    }
    //assume array is sorted
    //we find the middle value and compare it to our wanted value
    //if middle bigger; our element is in the first half
    //if middle smaller; our element is in the second half
    //if middle equal; we found our element
    //our half is now our whole array
    //repeat

    return NULL;
}

int RadixMap_add(RadixMap *map, uint32_t key, uint32_t value) {
    check(key < UINT32_MAX, "Key can't be greater or equal to UINT32_MAX.");

    RMElement element = { .data = { .key = key, .value = value } }; //create new element with key and value set (raw is comprised of both of those because we use union)
    check(map->end+1 < map->max, "RadixMap is full.");

    map->contents[map->end++] = element; //we add our element to the end of contents

    RadixMap_sort(map); //and we sort the array

    return 0;

error:
    return -1;
}

int RadixMap_delete(RadixMap *map, RMElement *el) {
    check(map->end > 0, "There is nothing to delete."); //if map epty or el==NULL, we cant do anything
    check(el != NULL, "Can't delete a NULL element.");

    el->data.key = UINT32_MAX; //we set elements key to max (our domain is until including max-1, so this is out of our domain (cant be searched or sorted))
    //DONT KNOW HOW THIS IS HELPING (POINTER STILL IN MAP.CONTENTS)

    if (map->end > 1) {
        // don't bother re-sorting a map of lenght 1
        RadixMap_sort(map); //we sort our new array without element
    }

    map->end--;

    return 0;

error:
    return -1;
}
