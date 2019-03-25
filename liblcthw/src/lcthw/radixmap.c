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
#include <inttypes.h> //for PRIu64 and PRIu32

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
//smaller y is the less important (righter byte) of the number

void print_count(uint64_t *count) {
    for (int i=0; i<256; i++) {
        printf("%d", count[i]);
    }
    printf("\n");
}

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
    //just imagine offset as digit in normal radix sort (start at the right (smaller/less imporant) digits and go to the left)


    // count occurneces of every byte value
    for (sp=source, end=source+max; sp<end; sp++) {
        count[ByteOf(sp, offset)]++;
    }
    //for each element in source (which is components array in RadixMap)
    //for current byte (offset)
    //we remember which number it represents
    //so at the end we have the number of all represented numbers for this offset
    //
    //so if we have numbers [123, 121, 100, 111] and offset 0
    //we would start with count 000000000 (for every didig from 0 to 9)
    //we would than count the occurences
    //so we would get count 1201000000
    //    one zero from 100
    //    two ones from 121 and 111
    //    and one three from 123
    //    others are zero
    //
    //if we would have offset 1 we would get count 1120000000
    //
    //we do this, but instead of didgits we have bytes and istead of 0-9 we have 0-256


    // tranform count into index by summing elements and storing into same array
    for (s=0, cp=count, end=count+256; cp<end; cp++) {
        c = *cp;
        *cp = s;
        s += c;
    }
    //we have s which is zero at the start
    //we go over every cell of count
    //we set that cell to the value of s
    //and we increment s by the original value of that cell
    //
    //so if we have count 1201000000
    //before -> 1201000000; s=0
    // 1st loop -> 0201000000; s=0+1=1
    // 2nd loop -> 0101000000; s=1+2=3
    // 3rd loop -> 0131000000; s=3+0=3
    // 4th loop -> 0133000000; s=3+1=4
    // 5th loop -> 0133400000; s=4+0=4
    // 6th loop -> 0133440000; s=4+0=4
    // 7th loop -> 0133444000; s=4+0=4
    // 8th loop -> 0133444400; s=4+0=4
    // 9th loop -> 0133444440; s=4+0=4
    //10th loop -> 0133444444; s=4+0=4


    // fill dest with the right values in the right place
    for (sp=source, end=source+max; sp<end; sp++) {
        cp = count + ByteOf(sp, offset); //count is the first element in count (= count[0])
        dest[*cp] = *sp;
        ++(*cp);
    }
    //for each element in source we calculate cp from its ByteOf and count to where it should be in dest
    //
    //so if we have source of [123, 121, 100, 111] and offset 0 (checking only the last digit)
    //dest can be full but we will overwrite it so lets just say its [0, 0, 0, 0] (at indecies 0, 1, 2 and 3)
    //we would get count 0133444444 from code above
    //
    //123:
    //so for 123, cp would be 0+3=3 (first element of count and first digit of 123)
    //so we would set element with index 3 in dest: [0, 0, 0, 0] -> [0, 0, 0, 123]
    //and we would increment element with index 3 in count: 0133444444 -> 0134444444
    //
    //121:
    //cp of 121: 0+1=1
    //we set element with index 1 in dest: [0, 0, 0, 123] -> [0, 121, 0, 123]
    //we increment element with index 1 in count: 0134444444 -> 0234444444
    //
    //100:
    //cp of 100: 0+0=0
    //set element with index 0 in dest: [0, 121, 0, 123] -> [100, 121, 0, 123]
    //and increment element with index 0 in count: 0234444444 -> 1234444444
    //
    //111:
    //and cp of 111: 1+1=2 (remember we changed the zeroth count to 1 in prev step)
    //set element with index 2 in dest: [100, 121, 0, 123] -> [100, 121, 111, 123]
    //and increment element wiht index 2 in count: 1234444444 -> 1244444444
    //
    //so our dest is [100, 121, 111, 123]
    //this is sorted by first (right) digit, not all the way
    //the elements with the same values stay in the same order (stable sort)
    //to get this all the way sorted we would need to sort by second and third digit (offset)
    //
    //(all of this is very smart)
}

void RadixMap_sort(RadixMap *map) {
    uint64_t *source = &map->contents[0].raw;
    uint64_t *temp = &map->temp[0].raw;

    radix_sort(0, map->end, source, temp); //sort source by first byte into temp
    radix_sort(1, map->end, temp, source); //sort temp (already sorted by first byte) by second byte into source
    radix_sort(2, map->end, source, temp); //repeat for third and fourth byte
    radix_sort(3, map->end, temp, source);
    //we change the order of source and temp so we dont need to copy from one to the other
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
    //if we exit the loop, it doesn't exist

    return NULL;
}

void print_element(RMElement *el) {
    printf("raw: %" PRIu64 ", key: %" PRIu32 ", value: %" PRIu32 "\n", el->raw, el->data.key, el->data.value);
}

int RadixMap_add(RadixMap *map, uint32_t key, uint32_t value) {
    check(key < UINT32_MAX, "Key can't be greater or equal to UINT32_MAX.");

    RMElement element = { .data = { .key = key, .value = value } }; //create new element with key and value set (raw is comprised of both of those because we use union)
    //print_element(&element);
    //raw is comprised of value(32 bits) directly followed by key (also 32 bits)
    //apparently structs work like that
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
    //
    //basically we set the key to the max possible value (but this is one more than its allowed for add and find)
    //when we sort after this, element will be pushed all the way to the end
    //but because we decrease the map->end after, it will seem as if there is nothing there
    //if we add new elements, they will just override our old deleted ones

    if (map->end > 1) {
        // don't bother re-sorting a map of lenght 1
        RadixMap_sort(map); //we sort our new array without element
    }

    map->end--; //we decrease end after sorting, so the deleted element is pushed to the end, and than we "snipp" it of

    return 0;

error:
    return -1;
}
