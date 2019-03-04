#include <stdio.h>
#include "ex22.h"
#include "dbg.h"

int THE_SIZE = 1000;

static int THE_AGE = 37;
//here statis is the opposite of extern
//static here means that this var is only to be used inside this .c and to not be accassable outside it
//(static here is based on the file level and can be different in other places)

int get_age() {
    return THE_AGE;
}

void set_age(int age) {
    THE_AGE = age;
}

double update_ratio(double new_ratio) {
    static double ratio = 1.0;
    //here the static variable inside a function is not accessable outside this function
    //this creates a constant state inside a function (can not be modified from without) (not common because of threads)
    //because the var is set to static, it isn't overwritten everytime the function is called (it retains its value even when the function exits and the stack is toppled)

    double old_ratio = ratio;
    ratio = new_ratio;

    return old_ratio;
}

void print_size() {
    log_info("I think the size is: %d", THE_SIZE);
}

/*
THE_SIZE is declared exterm so it can ne modified and accessed by other .c files
we also have a THE_AGE variable which is static and cannnot be modified outside this .c file,
    but we have funtions get_age() and set_age() that are within this file and can be accessed from outside this .c file so THE_AGE can be accessed and modified through those functions
update_ratio() takes new_ratio and returns the old. it uses a function level static variable to keep track of what the current ratio is
and print_size() prints out what ex22.c thinks THE_SIZE is currrently.
*/