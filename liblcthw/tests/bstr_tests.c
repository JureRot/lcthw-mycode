#include "minunit.h"
#include <lcthw/bstrlib.h>
#include "../src/lcthw/bstrlib.h" //just for writing

char *bfromcstr_test() {
    //create a bstring from a C style constant

    return NULL;
}

char *blk2bstr_test() {
    //do the same thing, but give the length of buffer

    return NULL;
}

char *bstrcpy_test() {
    //copy a bstring

    return NULL;
}

char *bassign_test() {
    //set one bstring to another

    return NULL;
}

char *bassigncstr_test() {
    //set a bstring to a C string's contents

    return NULL;
}

char *bassignblk_test() {
    //set a bstring to a C string, but give the length

    return NULL;
}

char *bdestroy_test() {
    //destroy a bstring

    return NULL;
}

char *bconcat_test() {
    //concatinate one bstring to another

    return NULL;
}

char *bstricmp_test() {
    //compare two bstrings returning the same result as strcmp

    return NULL;
}

char *biseq_test() {
    //test if two bstrings are equal

    return NULL;
}

char *binstr_test() {
    //tell if one string is in another

    return NULL;
}

char *bfindreplace_test() {
    //find one bstring in another and replace with a third

    return NULL;
}

char *bsplit_test() {
    //split a bstring into a bstrList

    return NULL;
}

char *bformat_test() {
    //do a format string, which is super handy

    return NULL;
}

char *blength_test() {
    //get the lenght of bstring

    return NULL;
}

char *bdata_test() {
    //get the date from a bstring

    return NULL;
}

char *bchar_test() {
    //get the char from a bstring

    return NULL;
}

char *all_test() {
    mu_suite_start();

    mu_run_test(bfromcstr_test);
    mu_run_test(blk2bstr_test);
    mu_run_test(bstrcpy_test);
    mu_run_test(bassign_test);
    mu_run_test(bassigncstr_test);
    mu_run_test(bassignblk_test);
    mu_run_test(bdestroy_test);
    mu_run_test(bconcat_test);
    mu_run_test(bstricmp_test);
    mu_run_test(biseq_test);
    mu_run_test(binstr_test);
    mu_run_test(bfindreplace_test);
    mu_run_test(bsplit_test);
    mu_run_test(bformat_test);
    mu_run_test(blength_test);
    mu_run_test(bdata_test);
    mu_run_test(bchar_test);

    return NULL;
}

RUN_TESTS(all_test);
