#include <stdio.h>

int main (int argc, char *argv[]) {
    //create two arrays we care about
    int ages[] = { 23, 42, 12, 89, 2 };
    char *names[] = { "Alan", "Frank", "Mary", "John", "Lisa" };
    
    //safely get the size of ages
    int count = sizeof(ages) / sizeof(int);
    //int i = 0;
    
    //first way using indexing
    for (int i=0; i<count; i++) {
        printf("%s has %d years alive.\n", names[i], ages[i]);
    }
    
    //printf("%p\n", ages); //arrays are actually pointers to the beginning of that memory block

    printf("---\n");


    //set up the pointers to the start of the arrays
    int *cur_age = ages; //pointer of the beginning of ages (array housing ints)
    char **cur_name = names; //point of the beginning of names (array of pointers of char arrays)

    //second way using pointers
    for (int i=0; i<count; i++) {
        printf("%s is %d years old.\n", *(cur_name + i), *(cur_age + i));
    }

    printf("---\n");

    //third way, pointers are just arrays
    for (int i=0; i<count; i++) {
       printf("%s is %d years old again.\n", cur_name[i], cur_age[i]);
    }

    printf("---\n");

    //fourth way with pointer in a stupid complex way
    for (cur_name=names, cur_age=ages; (cur_age - ages)<count; cur_name++, cur_age++) {
        printf("%s lived %d years so far.\n", *cur_name, *cur_age);
    }

    return 0;

    /*
     * type *ptr - A pointr of type named ptr
     *      int *ptr = bla; creates a pointer named ptr that points to the bla array which houses int types
     * *ptr - The value of whatever the ptr is pointed at
     *      prt houses the addres of where the value is stored. Thus ptr returns the address, *ptr returns the value at that address
     * *(ptr + i) - The value of (whatever the ptr is pointed at plus i)
     *      ptr[i] is the same as the above, but easier to read (pointers act kind of like arrays)
     * &thing - The address of thing
     *      &thing returns the location (address) of where the var is stored in memory
     *      if the variable is not array we need to use & to get its address (int *ptr = &variablename;)
     *      arrays are already sort of like pointers (location of the first value) so we dont need that (int *ptr = arrayname;)
     * type *ptr = &thing - A pointer of type named ptr set to the address of thing
     *      thing is class type and we get its address in memory with &. Than we create pointer ptr that points to that address.
     *      Thus ptr will house the location in memory of thing which is class type
     * ptr++ - Increment where ptr points
     *      this is not actually plus 1, but plus word size (word being word in memory) (or type size (sizeof(int)))
     *      Thus increment will point to the next element
     */
}
