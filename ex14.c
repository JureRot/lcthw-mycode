#include <stdio.h>
#include <ctype.h> //for funcs isalpha and isblank

//forward declarations
void print_arguments(int argc, char *argv[]);
void print_letters(char arg[]);
int can_print_it(char ch);
//this could be left out by writing functions in the right order
//but some cyclical calls could still need this forward delaration

void print_arguments(int argc, char *argv[]) {
    //int i = 0;
    for (int i=0; i<argc; i++) {
        print_letters(argv[i]);
    }
}

void print_letters(char arg[]) {
    //int i = 0;
    for (int i=0; arg[i]!='\0'; i++) {
        char ch = arg[i];

        if (can_print_it(ch)) {
            printf("'%c' == %d ", ch, ch);
            //prints a char and its ascii representation
        }
    }
    printf("\n");
}

int can_print_it(char ch) {
    return isalpha(ch) || isblank(ch);
}

int main (int argc, char *argv[]) {
    print_arguments(argc, argv);

    return 0;
}
