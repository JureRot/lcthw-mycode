#include <stdio.h>
#include <string.h>
#include "dbg.h"
#include "duffsdev.h"

//macros can also be defined here (in the header) instead of in the header file

int main(int argc, char *argv[]) {
    int count = 1000;
    char from[1000+1] = { 'a' };
    char to[1000+1] = { 'c' };

    memset(from, 'x', count);
    
    memset(to, 'y', count);
    log_info("%s\n", to);
    duff8(from, to, count);
    log_info("%s\n", to);

    printf("\n");

    memset(to, 'y', count);
    log_info("%s\n", to);
    duff16(from, to, count);
    log_info("%s\n", to);
    
    printf("\n");

    memset(to, 'y', count);
    log_info("%s\n", to);
    duff32(from, to, count);
    log_info("%s\n", to);
    
    //i dont think i quite understand the cpp. the macro becomes code, which is run, no returns, no setting
    //simillar to functional programming, just a black box (i think)

    return 0;
}