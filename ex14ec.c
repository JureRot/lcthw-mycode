#include <stdio.h>
#include <ctype.h> //for funcs isalpha and isblank
#include <string.h> //for strlen()

//forward declarations
void print_arguments(int argc, char *argv[]);
void print_letters(char arg[], int n);
int can_print_it(char ch);
//this could be left out by writing functions in the right order
//but some cyclical calls could still need this forward delaration

void print_arguments(int argc, char *argv[]) {
    //int i = 0;
    for (int i=0; i<argc; i++) {
        int len_str = strlen(argv[i]); //not the safest thing ever
        print_letters(argv[i], len_str);
    }
}

void print_letters(char arg[], int n) {
    //int i = 0;
    for (int i=0; i<n; i++) {
        char ch = arg[i];

        if (isalpha(ch) || isblank(ch)) {
            printf("'%c' == %d ", ch, ch);
            //prints a char and its ascii representation
        }
    }
    printf("\n");
}

int main (int argc, char *argv[]) {
    print_arguments(argc, argv);

    return 0;
}
