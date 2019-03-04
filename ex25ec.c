#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include "dbg.h"

#define MAX_DATA 100

int read_string(char **out_string, int max_buffer) {
    *out_string = calloc(1, max_buffer + 1); //here we set the pointe (with the allocated space)
    check_mem(*out_string);

    char *result = fgets(*out_string, max_buffer, stdin);
    check(result != NULL, "Input error.");

    return 0;

error:
    if (*out_string) {
        free(*out_string);
    }
    out_string = NULL;
    return -1;
}

int read_int(int *out_int) {
    char *input = NULL;
    int rc = read_string(&input, MAX_DATA);
    check(rc == 0, "Failed to read number.");

    *out_int = atoi(input);

    free(input);
    return 0;

error:
    if (input) {
        free(input);
    }
    return -1;
}

int read_scan(const char *fmt, ...) {
    int rc = 0;
    int *out_int = NULL;
    char *out_char = NULL;
    char **out_string = NULL;
    int max_buffer = 0;

    va_list argp;
    va_start(argp, fmt);

    for (int i=0; fmt[i]!='\0'; i++) {
        if (fmt[i] == '%') {
            i++;
            switch (fmt[i]) {
                case '\0':
                    sentinel("Invalid format, you ended with %%.");
                    break;

                case 'd':
                    out_int = va_arg(argp, int *);
                    rc = read_int(out_int);
                    check(rc == 0, "Failed to read int.");
                    break;

                case 'c':
                    out_char = va_arg(argp, char *);
                    *out_char = fgetc(stdin);
                    break;

                case 's':
                    max_buffer = va_arg(argp, int);
                    out_string = va_arg(argp, char **);
                    rc = read_string(out_string, max_buffer);
                    check(rc == 0, "Failed to read string.");
                    break;

                default:
                    sentinel("Invalid format.");
            }
        } else {
            fgetc(stdin);
        }
        check(!feof(stdin) && !ferror(stdin), "Input error.");
    }

    va_end(argp);
    return 0;

error:
    va_end(argp);
    return -1;
}

void my_print(char *str, ...) {
    //i go over input string char by char until we come to %
    //when we come to % we check what is next char (d, c, s) and get that from vargs

    int *int_arg = NULL;
    char num_str[MAX_DATA] = { '\0' }; //we need reserved space (cant init it inside switch)
    char *char_arg = NULL;
    char *str_arg = NULL;
    //this has to be declare beforehand, we cant declare new vars inside switch (because of how it works in c (acutal jump distances))

    va_list argp;
    va_start(argp, str);


    for (int i=0; str[i]!='\0'; i++) {
        if (str[i] == '%') {
            i++;
            switch (str[i]) {
                case '\0': //if null char -> we passed the wrong format
                    sentinel("Invalid format, you ended with %%.");
                    break;

                case 'd': //if d -> we expect number pointer in vargs
                    int_arg = va_arg(argp, int *); //we get that number pointer
                    sprintf(num_str, "%d", *int_arg); //this is how we convert to string
                    fputs(num_str, stdout); //and than we put that string to the stdout
                    break;

                case 'c': //if c -> we expect character pointer in vargs
                    char_arg = va_arg(argp, char *); //we get that char pointer
                    fputc(*char_arg, stdout); //and put it to the stdout
                    break;

                case 's': //if s -> we expect string in vargs
                    str_arg = va_arg(argp, char *); //we get that strin
                    fputs(str_arg, stdout); //and put it on the stdou
                    break;

                default: //anythign else is a invalid foramt
                    sentinel("Invalid format.");
            }
        } else { //if any of the other character, we just output it
            fputc(str[i], stdout);
        }
    }

    va_end(argp);

error:
    va_end(argp);
}

int main(int argc, char *argv[]) {
    char *first_name = NULL;
    char initial = ' ';
    char *last_name = NULL;
    int age = 0;

    my_print("What's your first name? ");
    int rc = read_scan("%s", MAX_DATA, &first_name);
    check(rc == 0, "Failed first name.");

    my_print("What's your initial? ");
    rc = read_scan("%c\n", &initial);
    check(rc == 0, "Failed initial.");

    my_print("What's your last name? ");
    rc = read_scan("%s", MAX_DATA, &last_name);
    check(rc == 0, "Failed last name.");

    my_print("How old are you? ");
    rc = read_scan("%d", &age);
    check(rc == 0, "Failed age.");

    my_print("---- RESULTS ----\n");
    my_print("First name: %s", first_name);
    my_print("Intial: %c\n", &initial); //for my implementation the %d and %c need to be a pointer
    my_print("Last name: %s", last_name);
    my_print("Age: %d\n", &age);

    free(first_name);
    free(last_name);

    //and this is just some testing
    char a[] = "bla";
    char b = '!';
    int c = 9001;
    my_print("Jure %s Rot%c, %d!!!\n", &a, &b, &c);

    return 0;

error:
    return -1;

}
