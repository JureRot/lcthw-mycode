#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <glob.h>
#include "dbg.h"

#define MAX_DATA 1024

int file_traverse(int is_or, int count, char *patterns[], char *filename) { // return -1 if error, 0 if no match, 1 if match
    FILE *file;
    char line[MAX_DATA];
    int matches[count];
    memset(matches, 0, sizeof(matches)); //init matches to all zeros
    int num_matches = 0;

    file = fopen(filename, "r");
    check(file != NULL, "Failed opening %s", filename);

    while (fgets(line, MAX_DATA, file) != NULL) {
        line[strlen(line)-1] = '\0';
        for (int i=0; i<count; i++) {
            if (strstr(line, patterns[i]) != NULL) {
                if (matches[i] == 0){ // if not repeated match
                    matches[i] = 1;
                    num_matches++;

                    if (is_or) { //if or setting, we found one, which is enough
                        goto match;
                    } else {
                        if (num_matches == count) { // if num_matches same as patterns, we found all
                            goto match;
                        }
                    }
                }
            }
        }
    }

    fclose(file);
    return 0;

match:
    if (file) {
        fclose(file);
    }
    return 1;

error:
    if (file) {
        fclose(file);
    }
    return -1;
}

int logfind_traverse(int is_or, int count, char *patterns[]) {
    FILE *logfind;
    char line[MAX_DATA];
    glob_t result;
    int ft = 0;

    logfind = fopen(".logfind", "r");
    check(logfind != NULL, "Failed opening .logfind");

    while (fgets(line, MAX_DATA, logfind) != NULL) {
        line[strlen(line)-1] = '\0'; //replace /n with \0

        glob(line, 0, 0, &result);
        for (int i=0; i<result.gl_pathc; i++) {
            ft = file_traverse(is_or, count, patterns, result.gl_pathv[i]);
            check(ft >= 0, "Error reading file %s", result.gl_pathv[i]);
            if (ft) {
                printf("%s\n", result.gl_pathv[i]);
            }
        }
        globfree(&result);
    }


    fclose(logfind);
    return 0;
error:
    if (logfind) {
        fclose(logfind);
    }
    return -1;
}

int main(int argc, char *argv[]) {
    FILE *in;
    //char in_line[MAX_DATA];

    in = fopen(".logfind", "r");
    check(in != NULL, "Couldn't open .logfind");

    if (argc < 2) {
        log_err("USAGE: logfind [-o] <search_string> [ ... ]");
    } else {
        if (strcmp(argv[1], "-o") == 0) {
            if (argc < 3) {
                log_err("USAGE: logfind [-o] <search_string> [ ... ]");
            } else {
                char *patterns[argc-2];
                for (int i=2; i<argc; i++) {
                    patterns[i-2] = argv[i];
                }
                logfind_traverse(1, argc-2, patterns);
            }
        } else {
            char *patterns[argc-1];
            for (int i=1; i<argc; i++) {
                patterns[i-1] = argv[i];
            }
            logfind_traverse(0, argc-1, patterns);
        }
    }

    fclose(in);
    return 0;

error:
    if (in) {
        fclose(in);
    }
    return -1;
}
