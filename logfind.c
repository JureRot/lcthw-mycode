#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <glob.h>
#include "dbg.h"

#define MAX_DATA 1024

//goes over every line of filename and sees if all/any (depending on is_or) of the patters match
//returns 0 if no match, 1 if match, -1 if error
int file_traverse(int is_or, int count, char *patterns[], char *filename) {
    FILE *file;
    char line[MAX_DATA];
    int matches[count];
    memset(matches, 0, sizeof(matches)); //init matches to all zeros
    int num_matches = 0;

    file = fopen(filename, "r");
    check(file != NULL, "Failed opening %s", filename);

    while (fgets(line, MAX_DATA, file) != NULL) {
        line[strlen(line)-1] = '\0'; // replaces \n at the end with \0
        for (int i=0; i<count; i++) {
            if (strstr(line, patterns[i]) != NULL) { // this is case sensitive (use strcasestr() for case insensitive)
                if (matches[i] == 0){ // if not a repeated match
                    matches[i] = 1;
                    num_matches++;

                    if (is_or) { // if or setting, we found one, which is enough
                        goto match;
                    } else {
                        if (num_matches == count) { // if and setting, if num_matches same as patterns, we found all
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

//goes over evey line in .logfind (whis is a regex pattern) and gets all filename matches using glob
//for each match, calls file_traverse and and prints out the filename if there was a match
//return 0, -1 if error
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
    if (argc < 2) {
        log_err("USAGE: logfind [-o] <search_string> [ ... ]");
    } else {
        if (strcmp(argv[1], "-o") == 0) {
            if (argc < 3) {
                log_err("USAGE: logfind [-o] <search_string> [ ... ]");
            } else {
                /*
                char *patterns[argc-2];
                for (int i=2; i<argc; i++) {
                    patterns[i-2] = argv[i];
                }
                */
                argc -= 2; //we decrease the count and skip the first two call arguments (./logfind, -o)
                argv += 2;
                logfind_traverse(1, argc, argv);
            }
        } else {
            /*
            char *patterns[argc-1];
            for (int i=1; i<argc; i++) {
                patterns[i-1] = argv[i];
            }
            */
            argc--; // we decrease the cound and skip the first argument (./logfind)
            argv++;
            logfind_traverse(0, argc, argv);
        }
    }

    return 0;

}

//my approach is a bit different from zed's
//i start in main and go to into another function (logfind_traverse) and from there i go even deeper into another function (file_traverse)
//so i basically just do some basic parsing in main, but than i hop to logfind_traverse
//there i find all files for every pattern (everyline) and execute file_travers on them
//(i construct a list of all files for one line of .logfind and call file_traverse on them)

//zed's approach is a bit different
//he first constructs a list of all files for all lines of .logfind
//and than calls "file_traverse" on them

//he goes only one level deep (main -> logfind_traverse -> main (with list) -> file_traverse)
//i go two levels deep (main -> logfind_traverse -> file_traverse -> logfind_traverse -> level_traverse ...)

//in my case the real computing is done between logfind_traverse and file_traverse and main has pretty much nothing to do with it (main just starts all of it)
//in his case, the main is the function that actually ties both lgofind_traverse and file_traverse together (indstead of just passing the program to them)

//(i think his approach is more elegant, but has some more complications (changing flags to append to file names using glob, allocating strings and passing the pointers around) and in the end, mine is the one that made the most sense for me in that moment)

// i dont know which approach is better (to to the integral part in main or to branch it out to other fucntions)
