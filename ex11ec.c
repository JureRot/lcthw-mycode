#include <stdio.h>

int main (int argc, char *argv[]) {

    int numbers[4] = { 0 }; //auto fills all others with zeros
    char *name[4] = { 'a' };

    // first, print them out raw
    printf("numbers: %d %d %d %d\n", 
            numbers[0], numbers[1], numbers[2], numbers[3]);
    
    printf("numbers: %c %c %c %c\n", 
            name[0], name[1], name[2], name[3]);

    printf("name: %s\n", name);

    // set up the numbers
    numbers[0] = '1';
    numbers[1] = '2';
    numbers[2] = '3';
    numbers[3] = '4';
    // if we try to print characters as int (%d) we get ascii codes

    // set up name
    name[0] = 'R';
    name[1] = 'o';
    name[2] = 't';
    name[3] = '\0';

    // then print them out initilaized
    printf("numbers: %d %d %d %d\n", 
            numbers[0], numbers[1], numbers[2], numbers[3]);
    
    printf("numbers: %c %c %c %c\n", 
            name[0], name[1], name[2], name[3]);

    //print the name like a string
    printf("name: %s\n", name);

    // another way to use name
    char *another = "Rot"; //this is a pointer to a string

    printf("another: %s\n", another);

    //c still lets us invoke each char in pointer
    printf("another each: %c %c %c %c\n", 
            another[0], another[1], another[2], another[3]);

    /*
     * void *memcpy(void *str1, const void *str2, size_t n)
     * copyies n characters from memory area str2 to memory area str1
     */
    return 0;
}
