#ifndef __duffsdev_h__
#define __duffsdev_h__

#include <stdio.h>
#include <errno.h>
#include <string.h>

#define duff8(from, to, C) {\
{\
        char *F = from;\
        char *T = to;\
        int n=(C+7)/8;\
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

#define duff16(from, to, C) {\
{\
        char *F = from;\
        char *T = to;\
        int n=(C+15)/16;\
        switch (C % 16) {\
            case 0:\
                do {\
                    *T++ = *F++;\
                    case 15:\
                    *T++ = *F++;\
                    case 14:\
                    *T++ = *F++;\
                    case 13:\
                    *T++ = *F++;\
                    case 12:\
                    *T++ = *F++;\
                    case 11:\
                    *T++ = *F++;\
                    case 10:\
                    *T++ = *F++;\
                    case 9:\
                    *T++ = *F++;\
                    case 8:\
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

#define duff32(from, to, C) {\
{\
        char *F = from;\
        char *T = to;\
        int n=(C+31)/32;\
        switch (C % 32) {\
            case 0:\
                do {\
                    *T++ = *F++;\
                    case 31:\
                    *T++ = *F++;\
                    case 30:\
                    *T++ = *F++;\
                    case 29:\
                    *T++ = *F++;\
                    case 28:\
                    *T++ = *F++;\
                    case 27:\
                    *T++ = *F++;\
                    case 26:\
                    *T++ = *F++;\
                    case 25:\
                    *T++ = *F++;\
                    case 24:\
                    *T++ = *F++;\
                    case 23:\
                    *T++ = *F++;\
                    case 22:\
                    *T++ = *F++;\
                    case 21:\
                    *T++ = *F++;\
                    case 20:\
                    *T++ = *F++;\
                    case 19:\
                    *T++ = *F++;\
                    case 18:\
                    *T++ = *F++;\
                    case 17:\
                    *T++ = *F++;\
                    case 16:\
                    *T++ = *F++;\
                    case 15:\
                    *T++ = *F++;\
                    case 14:\
                    *T++ = *F++;\
                    case 13:\
                    *T++ = *F++;\
                    case 12:\
                    *T++ = *F++;\
                    case 11:\
                    *T++ = *F++;\
                    case 10:\
                    *T++ = *F++;\
                    case 9:\
                    *T++ = *F++;\
                    case 8:\
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


//#define duff8(F, T, C) { T[0]=F[0]; }
//I dont think this is possible, i dont think it will let me edit outside values through pointers in preprocessor
//i get a bus error or a segfault (looks like we cant change outside the scope that easy)
//look into it
//[https://www.brainbell.com/tutors/c/Advice_and_Warnings_for_C/Macros_and_Miscellaneous_Pitfalls.html]

//BUT I WOULD REALLY LIKE TO DO THIS WITH THE MACRO, AND IN GENERALLY TO UNDERSTAND THIS HEADER FILES STUFF

//there is something called __inline (inline functions) (maybe look at that or something)

//I FOUND A WAY
//it wont let you assing or change the existing poitner, but you can create your own pointer (inside the scope of the macro) and use it to change the values of the actual positions (i believe)
//FUCK YEAH, WE DID IT, I HOPE ITS SAFE

#endif