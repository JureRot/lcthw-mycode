#include "ex22.h"
#include "dbg.h"

const char *MY_NAME = "Zed A. Shaw";
//alternative for using #define

void scope_demo(int count) {
    log_info("count is: %d", count);

    if (count > 10) {
        int count = 100; // BAD! BUGS!
        //there are two vars named count in differen scopes (function and if-statement)
        //c know which is which, but you dont (this is called shadowing a variable)
        log_info("count in this scope is: %d", count);
    }

    log_info("count at exit is: %d", count);

    count = 3000;

    log_info("count after assign is: %d", count);

    //because we pass the acutall number to this function even if we change, the changes will be within function scope
    //if we were to pass it a pointer, we would be able to change the value outside the function
    //this is pass by value vs pass by reference

    //Extra Credit
    //scope_demo(count);
    //this would result in a stackoverflow or a segfault
}

int main(int argc, char *argv[]) {
    // test out THE_AGE accessors
    log_info("My name: %s, age: %d", MY_NAME, get_age());

    set_age(100);

    log_info("My age is now: %d", get_age());

    // test out THE_SIZE extern
    log_info("THE_SIZE is: %d", THE_SIZE);
    print_size();

    THE_SIZE = 9;

    log_info("THE_SIZE is now: %d", THE_SIZE);
    print_size();
    //THE_SIZE also changes inside ex22.c

    // test the ratio function static
    log_info("Ratio at fist: %f", update_ratio(2.0));
    log_info("Ratio again: %f", update_ratio(10.0));
    log_info("Ratio once more: %f", update_ratio(300.0));

    // test the scope demo
    int count = 4;
    scope_demo(count);
    scope_demo(count * 20);

    log_info("count after calling scope_demo: %d", count);

    return 0;

    /*
        Don't shadow a variable inside another scope (like we do with count in scope_demo())
        Don't use too many global vars, and if you have to use them, use accessor functions (setters and getters)
            more like THE_AGE, instead of like THE_SIZE (doesn't apply for constants, because they are read only)
        When in doubt, put it on the heap
        Dont' use function static vars (ratio in update_ratio) (dont wive well with concurrency in threads)
            also, they are hard to find comparred to a well-done global variable
        Avoid reusing function parameters (calling scope_demo(count) one after another)
            it's confusing wether you are just reusing or changing the caller's version of it

        these are "just" rules and can be broken
    */

   //LOOK MORE INTO HEADER FILES AND SEPERATE FUNCTION DECLARATIONS
}