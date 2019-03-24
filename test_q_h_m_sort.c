#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef int (*cmp) (const void *a, const void *b);

int main(int argc, char *argv[]) {

    for (int i=0; i<argc; i++) {
        printf("%d %s\n", i, argv[i]);
    }

    heapsort(argv, argc, sizeof(char *), (cmp)strcmp);

    printf("sorted\n");
    for (int i=0; i<argc; i++) {
        printf("%d %s\n", i, argv[i]);
    }

    return 0;
}
