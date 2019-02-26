#include <stdio.h>
#include <string.h>
#include "dbg.h"
#include "duffsdev.h"

int main(int argc, char *argv[]) {
    char from[6] = { 'a','b','c','d','e','\0' };
    char to[6] = { 'a','a','a','a','a','\0' };
    int count = 6;

    printf("%s\n", to);
    int rc = duff8();
    printf("%s\n", to);
    printf("%d\n", rc);

    //i dont think i quite understand the cpp. the macro becomes code, which is run, no returns, no setting
    //simillar to functional programming, just a black box (i think)

    return 0;
}