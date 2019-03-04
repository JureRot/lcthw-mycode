#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "dbg.h"

#define MAX_DATA 100

int read_string(char **out_string, int max_buffer) {
    //we get pointer to a string (which is a pointer to a char array) and the max possible size of that string
    //this string is empty or it doesnt even exitst
    *out_string = calloc(1, max_buffer + 1); //here we set the pointe (with the allocated space)
    //we allocate the space for it (calloc is like malloc, but it fills the memory with zeros)
    check_mem(*out_string);

    char *result = fgets(*out_string, max_buffer, stdin); //here we set the value of the string (we set the buffer at the location of the poointer we allocated above)
    //we get the stdin and put it as the value of string (modify the allocated space with the stdin string)
    check(result != NULL, "Input error.");

    return 0;
    //WE DONT FREE TE *out_string, BECAUSE TI HAS TO REMAIN ALLOCATED FOR OTHER FUNCTIONS TO READ IT (will free it in main() or in read_int())

error:
    if (*out_string) {
        free(*out_string);
    }
    out_string = NULL;
    return -1;
}

int read_int(int *out_int) {
    //gets a pointer to an int (whis is empty or it doesnt even exist)
    char *input = NULL;
    //we create a local string variable
    int rc = read_string(&input, MAX_DATA);
    //we use read_string to get the string on the stdin
    check(rc == 0, "Failed to read number.");

    *out_int = atoi(input);
    //we convert the input string to a number

    free(input); //here we can free the input string allocated in read_string becuase we used a local variable to allocate and we set input argument manualy (thus the input argument is not allocated)
    return 0;

error:
    if (input) {
        free(input);
    }
    return -1;
}

int read_scan(const char *fmt, ...) {
    //... signals to c that there may come any number of arguments of any type after the fmt
    //if using ..., there can be no argument after it (the ... must be the last)
    int rc = 0;
    int *out_int = NULL;
    char *out_char = NULL;
    char **out_string = NULL;
    int max_buffer = 0;

    va_list argp;
    //va_list is the data type for vargs (need stdarg.h)
    va_start(argp, fmt);
    //this is how we init the vargs var. we pass the name of the va_list and the name of the last argument (before ...)

    for (int i=0; fmt[i]!='\0'; i++) { //goes over the format until end of string
        if (fmt[i] == '%') { //if current = %
            i++; //go to the next char
            switch (fmt[i]) {
                case '\0': //if null char -> we passed the wrong format
                    sentinel("Invalid format, you ended with %%.");
                    break;

                case 'd': //if d -> we expect number pointer in vargs
                    out_int = va_arg(argp, int *);
                    //this is how we read the next varg. we pass the name of va_list and the type of data we are expecting (shooting blind here, many posibilities of crashes)
                    rc = read_int(out_int);
                    //we actually pass it an empty pointer (which will be modidfied in read_int)
                    check(rc == 0, "Failed to read int.");
                    break;

                case 'c': //if c -> we expect character pointer in vargs
                    out_char = va_arg(argp, char *);
                    *out_char = fgetc(stdin);
                    break;

                case 's': //if s -> we expect max_size of string and string pointer in vargs
                    max_buffer = va_arg(argp, int);
                    out_string = va_arg(argp, char **);
                    rc = read_string(out_string, max_buffer);
                    //we pass it pointer to an empty string which it will modify to equal the whats on the stdin
                    check(rc == 0, "Failed to read string.");
                    break;

                default: //anythign else is a invalid foramt
                    sentinel("Invalid format.");
            }
        } else { //if the format is not %_, we eat the caracters until it is (THIS BEHAVES WEIRDLY)
            fgetc(stdin);
        }
        check(!feof(stdin) && !ferror(stdin), "Input error."); //we crash if encounter error or endoffile on stdin
    }

    va_end(argp);
    return 0;

error:
    va_end(argp);
    return -1;
}

int main(int argc, char *argv[]) {
    char *first_name = NULL;
    char initial = ' ';
    char *last_name = NULL;
    int age = 0;

    printf("What's your first name? ");
    int rc = read_scan("%s", MAX_DATA, &first_name); //if we dont pass enought arguments, the program will be stuck waiting
    //read_scan() alters the actual input arguments (here we input the first_name which is NULL and it will set it to the value that is read from stdin). that's why we are using pointers, (or location of string &) so we arent changeing the strings, and can acutally change its value from within another function.
    check(rc == 0, "Failed first name.");

    printf("What's your initial? ");
    rc = read_scan("%c\n", &initial); //\n here becuse fgetc() eats the newline (i think, not sure)
    check(rc == 0, "Failed initial.");

    printf("What's your last name? ");
    rc = read_scan("%s", MAX_DATA, &last_name);
    check(rc == 0, "Failed last name.");

    printf("How old are you? ");
    rc = read_scan("%d", &age);
    check(rc == 0, "Failed age.");

    printf("---- RESULTS ----\n");
    printf("First name: %s", first_name);
    printf("Intial: %c\n", initial);
    printf("Last name: %s", last_name);
    printf("Age: %d\n", age);

    free(first_name);
    free(last_name);
    //here we need to free the both strings
    //even tho the number also allocates a string during the reading, but alaready frees it inside read_int()
    return 0;

error:
    return -1;

    //the thing with pointers here is a bit weird, but its all about passing the poiinter to a uninitialized string or int, so the function can change its value (because it has the actual pointer to that variable in memory)
    //if we were to pass the acutall string to the function, the function would create a copy of that string and put it on its scope, and even if we were to change it, the actual value of the string would not be effected, because the scope woud close and we can just assign strings left and right like that.
    //but if we have a pointer to the actual location of it in ram, we can change its value (but we need to wrap out heads around all the * and &)

    //the prefix out_ (eg. out_string) is used to clearly signal, even if this is an input argument, it has output effects (it will be modifeid and is the actuall thing we are interested in returning (even if the func actually returns int))
    //basically it is to signal, that that variable is changing during the function (like changing in actuall ram on the wider scope)
}
