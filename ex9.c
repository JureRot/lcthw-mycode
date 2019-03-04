#include <stdio.h>

int main(int argc, char *argv[]) {
    int i = 0;
    while (i < 25) {
        printf("%d\n", i);
        i++;
    }

    //Extra credit
    printf("\n");
    i = 25;
    while (i > 0) {
        printf("%d\n", i);
        i--;
    }

    return 0;
}
