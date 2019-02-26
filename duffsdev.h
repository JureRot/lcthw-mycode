#ifndef __duffsdev_h__
#define __duffsdev_h__

#include <stdio.h>
#include <errno.h>
#include <string.h>

#define duff8(C)  { int n = (C+7)/8; for (int i=0; i<n; i++) { printf("%d", i); }; printf("%d", n); }

#endif