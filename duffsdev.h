#ifndef __duffsdev_h__
#define __duffsdev_h__

#include <stdio.h>
#include <errno.h>
#include <string.h>

//#define N 4
int n;

#define duff8(F, T, C) { n=(C+7)/8;\
        printf("%d\n", n); }

#endif