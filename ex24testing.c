#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dbg.h"

#define MAX_DATA 1024
#define NUM_LINES 5

int main(int argc, char *argv[]) {
    FILE *in;
    FILE *out;
    char *filename = "IOin.txt";
    char line[MAX_DATA];
    char input[NUM_LINES][MAX_DATA];
    char **input_dyn[NUM_LINES];
    int c = 0;

    in = fopen(filename, "r");
    out = fopen("IOout.txt", "w");
    check(in != NULL, "Couldn't open file %s", filename);
    check(out != NULL, "Couldn't write to file IOout.txt");

    while (fgets(line, MAX_DATA, in) != NULL) {
        line[strlen(line)-1] = '\0'; //replace \n at the end with \0
        //static input
        strncpy(input[c], line, MAX_DATA);
        //input[c][strlen(line)-1] = '\0'; //already replaced the \n at the end
        //dynamic input
        input_dyn[c] = malloc(strlen(line)*sizeof(char));
        strncpy(input_dyn[c], line, strlen(line));
        c++;
        fprintf(out, "The line of len %lu was: %s\n", strlen(line), line);
    }


    fclose(in);
    fclose(out);

    printf("input: %lu, input[0]: %lu\n", sizeof(input), sizeof(input[0]));
    for (int i=0; i<(sizeof(input)/sizeof(input[0])); i++) {
        printf("input: %s\n", input[i]);
    }

    printf("\ninput_dyn: %lu, input_dyn[0]: %lu\n", sizeof(input_dyn), sizeof(input_dyn[0]));
    for (int i=0; i<(sizeof(input_dyn)/sizeof(input_dyn[0])); i++) {
        printf("input_dyn: %s\n", input_dyn[i]);
        free(input_dyn[i]);
    }

    //if we have a fixed size of input array, we dont need to free it (there is no heap alloc (strcpy doesnt allocate))
    //and of course, do the standard ec

    //what about if we had a dynamic size of array (probably with pointers, but how, try that too)
    //i kinda made it work with input_dyn, and it works and all, BUT, it does throw some warning when compiling (wrong strncpy pointer and wrong type %s when printing)
    //and still i dont know of a easy way of making array len NUM_LINES independen (assigning a dynamic size var after check doesnt bode well) (but for now its just easier to just declare a global NUM_LINES when doing something like that)

    return 0;
error:
    if (in) {
        fclose(in);
    }
    if (out) {
        fclose(out);
    }
    for (int i=0; i<(sizeof(input_dyn)/sizeof(input_dyn[0])); i++) {
        free(input_dyn[i]);
    }
    return 1;
}
