#include <stdio.h>

void print_func(int c, int *a, char **n) {
   for (int i=0; i<c; i++) {
       printf("func call: age of %s is %d\n", n[i], a[i]);
   }
}


int main (int argc, char *argv[]) {

    //create two arrays we care about
    int ages[] = { 23, 43, 12, 89, 2 };
    char *names[] = { "Alan", "Frank", "Mary", "John", "Lisa" };
    //this is an array of char pointers (which are locations of char arrays (strings)). two levels

    //safely get the size of ages
    int count = sizeof(ages) / sizeof(int);
    //int i = 0;
    
    //first way using indexing
    for (int i=0; i<count; i++) { 
        printf("%s has %d years alive.\n", names[i], ages[i]);
    }

    printf("---\n");

    //set up the pointers to the start of the array
    int *cur_age = ages;
    //you can force different types with cast int *cur_age = (int *) names;
    char **cur_name = names;
    
    //cur_age[0] = 99;
    //you can assign new values in arrays using pointer

    //second way using pointers
    for (int i=0; i<count; i++) {
        printf("%s is %d years old.\n", *(cur_name + i), *(cur_age + i));
    }

    printf("---\n");
    
    //third way, pointers are just arrays
    for (int i=count-1; i>=0; i--) { //from last to first (different direction)
       printf("%s is %d years old, again.\n", cur_name[i], cur_age[i]);
    }

    printf("---\n");

    //fourth way with pointers in a stupid complex way
    for (cur_name=names, cur_age=ages; (cur_age - ages)<count; cur_name++, cur_age++) {
        //the test here asks how far have we gone from the begining of the ages
        //  both cur_age and ages are by themselves the addreses in memory (thus this is possible)
        printf("%s lived %d years so far.\n", *cur_name, *cur_age);
        //we actually change the pointer value as we go (move it forward)
    }

    /* The Pointer Lexicon
     *
     * TYPE *ptr - A pointer of type TYPE named ptr
     *      int *ptr = bla; ptr houses the location in memory of integer varaible bla (not really (bla is array), but go with it)
     * *ptr - The value of whatever ptr is pointed at
     *      ptr returns location in memory, *ptr returns the value thats stored at that location
     * *(ptr + i) - The value of (whatever ptr is pointed at plus i)
     *      ptr[i]; is the same as the above (and this is much easier to read)
     * &thing - The address of thing
     *      get the location of variable thing in memory (not needed for arrays, because they are already pointers to the first value)
     * TYPE *ptr = &thing - A pointer of type TYPE named ptr set to the address of thing
     * ptr++ - Increment where ptr points
     *      not actually plus 1 but plus sizeof(pointer type) (effectively shifting to the location of the next element in memory)
     *      
     * ptr->elem - Use pointer prt to struct/union to get the elem
     *      ptr points to struct or union and to get the value of element elem we would do something like ptr.elem
     *      but because it is a pointer we cant do that but need to dereference it something like
     *      (*ptr).elem, but this is long and hard to write so the shorthand for this is ptr->elem
     */

    //ARRAYS ARE NOT THE SAME AS POINTERS
    //  they act pretty similarly and use the same notation but are not the same
    //  a pointer is just a number, the location in memory
    //  an array is a block of memory (with beginning and end and lenght), we just access it similarly as pointers (with the location of the first element)
   
    // Extra Credit
    //reasign them because we moved them in previous for
    cur_age = ages;
    cur_name = names;

    printf("---\n");

    for (int i=0; i<count; i++) {
        printf("location: %p, value: %s\nlocation: %p, value: %d\n", cur_name+i, cur_name[i], cur_age+1, cur_age[i]);
    }

    printf("---\n");

    print_func(count, cur_age, cur_name);

    return 0;
}
