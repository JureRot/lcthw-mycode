#ifndef _ex22_h
#define _ex22_h

// makes THE_SIZE in ex22.c available to other .c files
extern int THE_SIZE;
//extern means that the var exists, but it's in another "esternal" location
//tipcally this means that one .c file will use a variable that's been defined in another .c file (ex22_main.c will use THE_SIZE that's defined in ex22.c)
//(removing the extern doesnt seem to have an effect)

// gets and sets an internal static variable in ex22.c
int get_age();
void set_age(int age);

// updates a static variable that's inside update_ratio
double update_ratio(double ratio);

void print_size();

#endif