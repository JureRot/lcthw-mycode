#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dbg.h"

#define MAX_DATA 1024

int main(int argc, char *argv[]) {
    FILE *in;
    FILE *out;
    char *filename = "IOin.txt";
    char line[MAX_DATA];
    char **input;

    in = fopen(filename, "r");
    out = fopen("IOout.txt", "w");
    check(in != NULL, "Couldn't open file %s", filename);

    int c = 0;
    while (fgets(line, MAX_DATA, in) != NULL) {
        char *line_ptr = malloc(sizeof(line));
        memcpy(line_ptr, line, strlen(line));
        printf("%p, %s\n", *line_ptr, line_ptr);
        //input[c++] = line_ptr;
        fprintf(out, "The line was: %s", line);
        free(line_ptr);
    }

    fclose(in);
    fclose(out);


    //we still need to free every line in input)

    return 0;
error:
    return 1;
}