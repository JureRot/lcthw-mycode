#include <stdio.h>

int main (int argc, char *argv[]) {

    argv[1] = "Texas";

    // go throug each string in argv
    // why am i skipping argv[0]?
    for (int i=1; i<argc; i++) {
    //start with 1 because the 0-th will be "./ex13"
        printf("arg %d: %s\n", i, argv[i]);
    }

    // let's make our own array of strings
    char *states[] = { "California", NULL, "Washington", "Texas" };
    //actually creates an array of pointers to the strings (i believe)
    //a 2D array, an array of char arrays (which is equal to strings in c)

    int num_states = 4;

    for (int i=0; i<num_states; i++) {
        printf("state %d: %s\n", i, states[i]);
    }

    // printf("%c\n", states[0][4]);
    // we can acces the strings in char *array[] like this (this works)

    //interesingly there is no false and true, just 0 and anything else
    //another interesting thing: comma separator (,) in for and while loops
    //  executes the first commands but than ignores is for the second
    //  can be used like this while(i++,i<5){ for instance
   
    return 0;
}
