#include <stdio.h>
#include <stdlib.h>
#include "dbg.h"

#define MAX_DATA 100

typedef enum EyeColor {
    BLUE_EYES,
    GREEN_EYES,
    BROWN_EYES,
    BLACK_EYES,
    OTHER_EYES,
} EyeColor;
//this is something i dont get quite

const char *EYE_COLOR_NAME[] = {
    "Blue",
    "Green",
    "Brown",
    "Black",
    "Other"
};

typedef struct Person {
    int age;
    char first_name[MAX_DATA];
    char last_name[MAX_DATA];
    EyeColor eyes;
    float income;
} Person;

//removes newline char at the end of the string
void remove_nl(char *str) { //we can make this func void, because we actually modify the input string (dealing with pointers)
    if (str[strlen(str)-1] == '\n') {
        str[strlen(str)-1] = '\0';
    }
}
//takes a pointer to a char array (must be char array (char name[]) not a pointer to string (char *name)) and if the last non-nul characters is newline, it replaces it with another \0. This doesnt make the array shorter, just the last two characters are both \0 (which makes the string shorter strlen()). at the end returns the same pointer to the char array.

//slides all characters from n till the end one to the left (effectively removing n-th character and adding \0 to the end)
void char_slide_left(char *str, int n) {
    for (int i=n; i<strlen(str)-1; i++) {
        str[i] = str[i+1];
    }
    str[strlen(str)-1] = '\0';
}

//remove any whitespace inside the string
void remove_ws(char *str) {
    //this is harder, will need a new string using malloc
    //? OR NOT, maybe we can just move the characters after the whitepace one char to the left (for each whitespace)
    //and fill the end with \0 (add a new func, char_slide_left or something)
    //(idea for keeping the same string and just removing the whitespace is, so we dont need to free() at the end and we have no mem leaks)
    for (int i=0; i<strlen(str); i++) {
        if (str[i] == ' ') {
            char_slide_left(str, i); //this is not the safest thing (we are modifying the string we are currently for-ing over)
            i--; //we need this for when there are consecutive spaces (otherwise we would skip the second)
        }
    }
}

int main(int argc, char *argv[]) {
    Person you = { .age = 0 };
    //int i = 0;
    char *in = NULL;

    printf("What's your first name? ");
    in = fgets(you.first_name, MAX_DATA-1, stdin);
    //fgets(s, size, stream) reads at most size-1 characters from stream and stores them to buffer pointer to by s (if encounters EOF or newline it stops and writes \0 after the last char)
    check(in != NULL, "Failed to read first name.");
    //fscanf(stdin, "%50s", you.first_name);
    //its hard to use fscanf on strings, because we dont know in advance how big the buffer will be (so in this case it reads too much an east our <CR>)

    printf("What's your last name? ");
    in = fgets(you.last_name, MAX_DATA-1, stdin);
    //in = gets(you.last_name); //linux doesnt even have the gets() function
    check(in != NULL, "Failed to read last name.");

    printf("How old are you? ");
    in = fgets(in, MAX_DATA-1, stdin);
    check(atoi(in) != 0, "You have to enter a number."); //this isnt fool proof, it you can for instance enter 123abc456 and ti will recognize 123
    you.age = atoi(in);
    
    printf("What color are your eyes:\n");
    for (int i=0; i<=OTHER_EYES; i++) { //because EyeColors is a enum, its elements become enumerated, so the OHTER_EYES (the last element) is actually a number (of sorts)
        printf("%d) %s\n", i+1, EYE_COLOR_NAME[i]);
    }
    printf("> ");

    int eyes = - 1;
    in = fgets(in, MAX_DATA-1, stdin);
    check(atoi(in)!=0, "You have to enter a number");
    eyes = atoi(in);

    you.eyes = eyes - 1;
    check(you.eyes<=OTHER_EYES && you.eyes>=0, "Do it right, that's not an option.");
    
    printf("How much do you make an hour? ");
    in = fgets(in, MAX_DATA-1, stdin);
    check(atoi(in)!=0, "Enter a floating point number."); //again it will recognize 123abc456 as number
    you.income = atoi(in);

    printf("----- RESULTS -----\n");

    printf("First Name: %s", you.first_name);
    printf("Last Name: %s", you.last_name);
    printf("Age: %d\n", you.age);
    printf("Eyes: %s\n", EYE_COLOR_NAME[you.eyes]);
    printf("Income: %f\n", you.income);

    char bla1[] = "Jure\n";
    char bla2[] = "Jure"; //this is array of characters, characters inside this can be modified (the actual variable bla2 is a pointer to the first char in this array)
    char *bla3 = "Jure"; //this is a pointer to a string, characters inside this cannot be modified
    printf("%ld, %ld\n", sizeof(bla2), sizeof(bla3));
    printf("%ld, %ld\n", strlen(bla2), strlen(bla3));
    printf("%p, %p\n", bla2, bla3);

    printf("%s, %s\n", bla1, bla2);
    remove_nl(bla1); //this function actually modifies the input string
    remove_nl(bla2);
    printf("%s, %s\n", bla1, bla2);

    char bla4[] = "  Jure Rot   ";
    printf("%s\n", bla4);
    remove_ws(bla4); //this function actually modifies the input string
    printf("%s\n", bla4);

   //scanf() is the same as the fscanf(), except with fscanf() you have to pass the source, in scanf() the source is always stdin

    return 0;
error:
    return -1;
}
