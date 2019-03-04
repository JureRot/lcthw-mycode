#include <stdio.h>
#include <string.h>
#include "dbg.h"

//goes over every char in strings and copyies from from to to. remember these vars are in function scope (wont change the callers vars). at the end it returns the i (which shold be equal to count)
int normal_copy(char *from, char *to, int count) {
    int i = 0;
    for (i=0; i<count; i++) {
        to[i] = from[i];
    }
    return i;
}

//we play around with 2 values (n and count%8 (which i'll call rem)). count is equal to either ((n-1)*8 + rem) or (n*8 ; if rem=0)
//and the func does the following:
//  when we enter the switch, we first jump to the case for the remainder
//    (if remainder is 0 we jump to the beginning, acting like the remainder is 8)
//  the operation we are doing is assigning the value of pointer from to the value of pointer to and then incrementing both (first the assignent, than the incrment (which moves the pointers to the location of the next char in string))
//  because switch has no break statements, this operation is repeated till the end. (at fist this will be rem-times)
//  after the first walk over switch, the n is decreased and  and than compared if greater than 0. if yes (greater) we jump the the begining of the do-while loop. this means that we will do the whole do-while loop n-1 times.
//  (we can jump/loop inside a switch statement, becuase in C the switch statement is written in a solid block and the case locations are actually RAM jump amounts (look exercise about switch))
//
//so in all, we will change rem number of characters first and than go through whole do-while (n-1) times, which changes 8 characters each time. this will result in changing (rem+(n-1)*8 = count) count number of characters in total. again, count as a multiple of 8 is an exception (we won't have a remainder, but we will go through whole do-while loop n times, still resulting in count changes)
int duffs_device(char *from, char *to, int count) {
    { //i don't know why the whole function is inside a block (probably so the whole funct is written in a solid block, so we can jump around the switch statement like we do)
        int n = (count + 7) / 8; //+7 for if the count < 8
        //because it's cast to int is considered as whole-number didivision (no decimal) (same as floor)

        switch (count % 8) {
            case 0:
                do {
                    *to++ = *from++;
                    case 7:
                    *to++ = *from++;
                    case 6:
                    *to++ = *from++;
                    case 5:
                    *to++ = *from++;
                    case 4:
                    *to++ = *from++;
                    case 3:
                    *to++ = *from++;
                    case 2:
                    *to++ = *from++;
                    case 1:
                    *to++ = *from++;
                } while (--n > 0);
        }
    }
    return count;
}

//does pretty much the same as duffs_device, but instead of do-while loop, it uses goto jump command
int zeds_device(char *from, char *to, int count) {
    {
        int n = (count + 7) / 8;

        switch (count % 8) {
            case 0:
again:          *to++ = *from++;

            case 7:
                *to++ = *from++;
            case 6:
                *to++ = *from++;
            case 5:
                *to++ = *from++;
            case 4:
                *to++ = *from++;
            case 3:
                *to++ = *from++;
            case 2:
                *to++ = *from++;
            case 1:
                *to++ = *from++;
            if (--n > 0) {
                goto again;
            }
        }
    }
    return count;
}

//returns 0 if missmatch is found, otherwise returns 1
int valid_copy(char *data, int count, char expects) {
    for (int i=0; i<count; i++) {
        if (data[i] != expects) {
            log_err("[%d] %c != %c", i, data[i], expects);
            return 0;
        }
    }
    return 1;
}

int main(int argc, char *argv[]) {
    char from[1000] = { 'a' };
    char to[1000] = { 'c' };
    int rc = 0;

    // setup the from to have some stuff
    memset(from, 'x', 1000);
    //set it to a failure mode
    memset(to, 'y', 1000);
    check(valid_copy(to, 1000, 'y'), "Not initalized right.");

    // use normal copy to
    rc = normal_copy(from, to, 1000);
    check(rc == 1000, "Normal copy failed: %d", rc);
    check(valid_copy(to, 1000, 'x'), "Normal copy failed.");

    // reset
    memset(to, 'y', 1000);

    // Duffs version
    rc = duffs_device(from, to, 1000);
    check(rc == 1000, "Duffs device failed: %d", rc);
    check(valid_copy(to, 1000, 'x'), "Duffs device failed.");

    // reset
    memset(to, 'y', 1000);

    // Zeds version
    rc = zeds_device(from, to, 1000);
    check(rc == 1000, "Zeds device failed: %d", rc);
    check(valid_copy(to, 1000, 'x'), "Zeds device failed.");

    return 0;
error:
    return 1;
}