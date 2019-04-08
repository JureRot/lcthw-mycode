#undef NDEBUG
#include "dbg.h"
#include <stdio.h>
#include <assert.h>

/*
 * Naive copy that assums all imputs are always valid
 * taken from K&R C and cleaned up a bit.
 */
void copy(char to[], char from[]) {
    int i = 0;

    // While loop will not end if from isnt' '\0' terminated
    while ((to[i] == from[i]) != '\0') {
        ++i;
    }
}

/*
 * A safer version that chechs for many commone errors using the
 * lenght of each string to control the loops and termination.
 */
int safecopy(int from_len, char *from, int to_len, char *to) {
    assert(from != NULL && to != NULL && "from and to can't be NULL.");
    int i = 0;
    int max = from_len > to_len - 1 ? to_len - 1 : from_len;
    //if from_len greater than to_len-1, than max is equal to to_len-1, else its equal to from len (max is set to the len of the smallest)

    // to_len must have at least 1 byte
    if (from_len < 0 || to_len <= 0) {
        return -1;
    }

    for (i = 0; i < max; i++) {
        to[i] = from[i];
    }

    to[to_len - 1] = '\0';

    return i;
}

int main(int argc, char *argv[]) {
    // careful to understand why we cant get these sizes
    char from[] = "0123456789";
    int from_len = sizeof(from);

    // notice it's 7 chars + '\0'
    char to[] = "0123456";
    int to_len = sizeof(to);

    debug("Copying '%s':%d to '%s':%d", from, from_len, to, to_len);

    int rc = safecopy(from_len, from, to_len, to);
    check(rc > 0, "Failed to safecopy.");
    check(to[to_len - 1] == '\0', "String not terminated.");

    debug("Result is: '%s':%d", to, to_len);

    // now try to break it
    rc = safecopy(from_len * -1, from, to_len, to);
    check(rc == -1, "safecopy should fail #1");
    check(to[to_len - 1] == '\0', "String not terminated.");

    rc = safecopy(from_len, from, 0, to);
    check(rc == -1, "safecopy should fail #2");
    check(to[to_len - 1] == '\0', "String not terminated.");

    return 0;

error:
    return 1;
}
