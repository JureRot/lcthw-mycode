#include <stdio.h>
#include <glob.h>
#include "dbg.h"

int main(int argc, char *argv[]) {
    glob_t result;
    glob("*.c", 0, 0, &result);
    for (int i=0; i<result.gl_pathc; i++) {
        printf("%s\n", result.gl_pathv[i]);
    }
    globfree(&result);

    return 0;
}
