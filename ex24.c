#include <stdio.h>
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
    n = fgets(you.last_name, MAX_DATA-1, stdin);
    //in = gets(you.last_name); //linux doesnt even have the gets() function
    check(in != NULL, "Failed to read last name.");

    printf("How old are you? ");
    int rc = fscanf(stdin, "%d", &you.age);
    //fscanf(stream, format, ptr) reads from stream, converts it using format and saves it to the location of ptr (?basically?)
    check(rc > 0, "You have to enter a number.");
    
    printf("What color are your eyes:\n");
    for (int i=0; i<=OTHER_EYES; i++) { //because EyeColors is a enum, its elements become enumerated, so the OHTER_EYES (the last element) is actually a number (of sorts)
        printf("%d) %s\n", i+1, EYE_COLOR_NAME[i]);
    }
    printf("> ");

    int eyes = - 1;
    rc = fscanf(stdin, "%d", &eyes);
    check(rc > 0, "You have to enter a number.");

    you.eyes = eyes - 1;
    check(you.eyes<=OTHER_EYES && you.eyes>=0, "Do it right, that's not an option.");
    
    printf("How much do you make an hour? ");
    rc = fscanf(stdin, "%f", &you.income);
    check(rc > 0, "Enter a floating point number.");

    printf("----- RESULTS -----\n");

    printf("First Name: %s", you.first_name);
    printf("Last Name: %s", you.last_name);
    printf("Age: %d\n", you.age);
    printf("Eyes: %s\n", EYE_COLOR_NAME[you.eyes]);
    printf("Income: %f\n", you.income);

    return 0;
error:
    return -1;
    /*
        More f functions:
            fscanf
            fgets
            fopen
            freopen
            fdopen
            fclose
            fcloseall
            fgetpos
            fseek
            ftell
            frewind
            fprintf
            fwrite
            fread

        all of those have other versions, soo look into those too
    */

    //FOR YOU EXTRA CREDIT, READ AND WRITE TO AN ACTUAL .TXT FILE
}