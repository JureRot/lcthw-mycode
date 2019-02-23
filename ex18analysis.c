#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

char *bla(int c) {
    char *output = malloc(c * sizeof(char));

    for (int i=0; i<c; i++) {
        output[i] = i + '0';
    }
    return output;
}

void nekineki(int c, char *(*f) (int)) {
    //second arg is a pointer named f to a function that takes int and returns char * (weird syntax but ok)
    printf("Neki:\n");
    for (int i=0; i<c; i++) {
        char *num_string = f(c);
        printf("\t%s\n", num_string);
        free(num_string);
    }
}

typedef char *(*bla_func) (int);

void nekineki2(int c, bla_func f) {
    //on the other hand, if we typedef the function pointer, we can call it more like a variable type (and we get rid of the weird syntax)
    printf("Neki2:\n");
    for (int i=0; i<c; i++) {
        char *num_string = f(c);
        printf("\t%s\n", num_string);
        free(num_string);
    }
}

int main(int argc, char *argv[]) {
    //when we pass bla as the pointer to func, the compiler looks if the signatures match (input and return)
    //and if they do, it replaces bla with the addres in memory where the bla's code is written
    nekineki(6, bla);
    
    //we can typedef the function pointer to make it behave more like a variable
    //so when we pass it to other function it acts like we can actually pass fuctions to a function (but in reality we are passing a pointer to a function disguised in a custom type)
    nekineki2(6, bla);

    //and the third thing is "hard-assigning" the function to the function pointer
    printf("\n");
    char *(*tester) (int) = bla;
    char *tester_str = tester(3);
    char *bla_str = bla(3);
    
    printf("tester: %s is now same as bla: %s\n", tester_str, bla_str);
    
    free(tester_str);
    free(bla_str);

    return 0;
}
