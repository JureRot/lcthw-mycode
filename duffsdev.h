#ifndef __duffsdev_h__
#define __duffsdev_h__

#include <stdio.h>
#include <errno.h>
#include <string.h>

//#define N 4
int n;
char *temp;

/*
#define duff8(F, T, C) {\
{\
        n=(C+7)/8;\
        switch (C % 8) {\
            case 0:\
                do {\
                    *T++ = *F++;\
                    case 7:\
                    *T++ = *F++;\
                    case 6:\
                    *T++ = *F++;\
                    case 5:\
                    *T++ = *F++;\
                    case 4:\
                    *T++ = *F++;\
                    case 3:\
                    *T++ = *F++;\
                    case 2:\
                    *T++ = *F++;\
                    case 1:\
                    *T++ = *F++;\
                } while(--n > 0);\
        }\
}\
}
*/

#define duff8(F, T, C) { T[0]=F[0]; }

#endif