#include "bstrlib.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>

int main(int argc, char *argv[]) {

    char *str = "bla";
    bstring bstr;
    bstr = bfromcstr(str);

    printf("str: %s, bstr: %s\n", str, bstr->data);

    return 0;
}
