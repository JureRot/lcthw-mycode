#include <stdio.h>
#include <ctype.h>
#include "dbg.h"

int print_a_message(int len, const char *msg) {
    printf("A STRRING: %s\n", msg);

    return 0;
}

int uppercase(int len, const char *msg) {
    for (int i=0; i<=len; i++) {
        printf("%c", toupper(msg[i]));
    }

    printf("\n");

    return 0;
}

int lowercase(int len, const char *msg) {
    for (int i=0; i<=len; i++) {
        printf("%c", tolower(msg[i]));
    }

    printf("\n");

    return 0;
}

int fail_on_purpose(int len, const char *msg) {
    return 1;
}
