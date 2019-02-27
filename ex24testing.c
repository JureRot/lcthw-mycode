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
    //char *input[NUM_LINES];
    int c = 0;

    in = fopen(filename, "r");
    out = fopen("IOout.txt", "w");
    check(in != NULL, "Couldn't open file %s", filename);
    check(out != NULL, "Couldn't write to file IOout.txt");

    while (fgets(line, MAX_DATA, in) != NULL) {
        //input[c] = malloc(strlen(line));
        strncpy(input[c], line, MAX_DATA);
        input[c][strlen(line)-1] = '\0'; //replace \n with \0
        c++;
        fprintf(out, "The line of len %lu was: %s", strlen(line), line);
    }

    fclose(in);
    fclose(out);

    printf("%lu %lu\n", sizeof(input), sizeof(input[0]));
    for (int i=0; i<(sizeof(input)/sizeof(input[0])); i++) {
        printf("%s\n", input[i]);
    }

    //if we have a fixed size of input array, we dont need to free it (there is no heap alloc (strcpy doesnt allocate))
    //what about if we had a dynamic size of array (probably with pointers, but how, try that too)
    //and of course, do the standard ec

    return 0;
error:
    if (in) {
        fclose(in);
    }
    if (out) {
        fclose(out);
    }
    return 1;
}