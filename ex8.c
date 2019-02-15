#include <stdio.h>

int main(int argc, char *argv[]) {

    //int i = 0; //you can init the var inside of for (like java)

    if (argc == 1) {
        // printf("You have one argument. You suck.\n");
        printf("You have no arguments. You suck.\n"); //firs arg is name of file
    } else if (argc > 1 && argc < 4) {
        printf("Here's your arguments:\n");

        for(int i=1; i<argc; i++) { //start with 1 (first arg is file passed)
            printf("%s ", argv[i]);
        }
        printf("\n");
    } else {
        printf("You have to many arguments. You suck.\n");
    }

    return 0;
}
