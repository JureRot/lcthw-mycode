#include <stdio.h>
#include <string.h>
#include "dbg.h"
#include "duffsdev.h"

int main(int argc, char *argv[]) {
    char *from = "abcde";
    char *to = "xxxxx";
    int count = 5;

    printf("%s\n", to);
    duff8(from, to, count);
    printf("%s\n", to);

    //i dont think i quite understand the cpp. the macro becomes code, which is run, no returns, no setting
    //simillar to functional programming, just a black box (i think)

    return 0;
}