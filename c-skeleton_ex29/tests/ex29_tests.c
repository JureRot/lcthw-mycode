#include <stdio.h>
#include "dbg.h"
#include <dlfcn.h>
#include <errno.h>

typedef int (*lib_function) (const char *data); //this func pointer matches  the singnature of every func in libex29

int main(int argc, char *argv[]) {
    int rc = 0;
    check(argc == 4, "USAGE: ex29 libex29.so function data");

    char *lib_file = argv[1];
    char *func_to_run = argv[2];
    char *data = argv[3];

    void *lib = dlopen(lib_file, RTLD_NOW); //load the library by name lib_file (similar to opening the file)
    check(lib != NULL, "Failed to open the library %s: %s", lib_file, dlerror());

    lib_function func = dlsym(lib, func_to_run); //here we get a function out of lib_file only by its string name (this the the powerful thing)
    check(func != NULL, "Did not find %s function in the library %s: %s", func_to_run, lib_file, dlerror());

    rc = func(data); //we than call this function and pass it the data
    check(rc == 0, "Function %s returned %d for data: %s", func_to_run, rc, data);

    rc = dlclose(lib); //at the end we close the library
    check(rc == 0, "Failed to close %s", lib_file);

    return 0;

error:
    return 1;
}

/*
 * HOW TO RUN
 * # comile the lib file and make the .so (may need -fPIC on some platforms (add if you get an error))
 * $ cc -c libex29.c -o libex29.o
 * $ cc -shared -o libex29.so libex29.o
 * #we first make it to object file and than we make it to shared object file (dynamic library)
 *
 * # make the loader program
 * $ cc -Wall -g -DNDEBUG ex29.c -ldl -o ex29
 * #see how we dont link it an acutal library (-llibex20.a) but a dynamic library (-ldl)
 * #and the ordre of flags here is important
 *
 * # and run it
 * $ ./ex29 ./libex29.so print_a_message "Hello!"
 *
 * on mac the ! is a problem (-bash: ! event not found)
 */
