//may need more includes (stdio, stdlib, ....)
#include "minunit.h"
#include <lcthw/bstrlib.h>
//#include "../src/lcthw/bstrlib.h" //just for writing

char *bfromcstr_test() {
    //create a bstring from a C style constant
    char *str_full = "This is a test string.";
    char *str_empty = "";

    bstring bstr_full = bfromcstr(str_full);
    mu_assert(bstr_full != NULL, "bfromcstr on full string returned NULL");
    mu_assert(strcmp(str_full, bstr_full->data) == 0, "bfromcstr string is not equal to original full str.");

    bstring bstr_empty = bfromcstr(str_empty);
    mu_assert(bstr_empty != NULL, "bfromcstr on empty string returned NULL");
    mu_assert(strcmp(str_empty, bstr_empty->data) == 0, "bfromcstr string is not equal to original empty str.");

    bdestroy(bstr_full);
    bdestroy(bstr_empty);

    return NULL;
}

char *blk2bstr_test() {
    //do the same thing, but give the length of buffer
    char *str_full = "This is a test string2";
    int len_full = 22;
    char *str_empty = "";
    int len_empty = 0;

    bstring bstr_full = blk2bstr(str_full, len_full);
    mu_assert(bstr_full != NULL, "blk2bstr on full string returned NULL");
    mu_assert(strcmp(str_full, bstr_full->data) == 0, "blk2bstr string is not equal to original full str.");

    bstring bstr_empty = blk2bstr(str_empty, len_empty);
    mu_assert(bstr_empty != NULL, "blk2bstr on empty string returned NULL");
    mu_assert(strcmp(str_empty, bstr_empty->data) == 0, "blk2bstr string is not equal to original empty str.");

    bdestroy(bstr_full);
    bdestroy(bstr_empty);

    return NULL;
}

char *bstrcpy_test() {
    //copy a bstring
    bstring bstr = bfromcstr("This is a test bstring.");

    bstring bstr_copy = bstrcpy(bstr);
    mu_assert(bstr_copy != NULL, "bstcopy returned NULL");
    mu_assert(strcmp(bstr->data, bstr_copy->data) == 0, "copy of bstring is not the same as original.");

    bstr = bfromcstr("changed string");
    mu_assert(strcmp("This is a test bstring.", bstr_copy->data) == 0, "copy of bstring changes after changing original");

    //TODO, leakcheck
    bdestroy(bstr);
    mu_assert(strcmp("This is a test bstring.", bstr_copy->data) == 0, "copy of bstring changes after destroying original");

    bdestroy(bstr_copy);

    return NULL;
}

char *bassign_test() {
    //set one bstring to another
    bstring bstr = bfromcstr("This is a test bstring.");
    bstring testing_bstr = bfromcstr("starting value");

    mu_assert(strcmp(bstr->data, testing_bstr->data) != 0, "strings are equal before bassign");

    int rc = bassign(testing_bstr, bstr);
    mu_assert(rc == 0, "bassign returned error");
    mu_assert(testing_bstr != NULL, "bassign changed testing_bstr to NULL");
    mu_assert(strcmp(bstr->data, testing_bstr->data) == 0, "strings arent equal after bassign");

    bstr = bfromcstr("changed string");
    mu_assert(strcmp("This is a test bstring.", testing_bstr->data) == 0, "assigned changes after source is changed.");

    //TODO, leakcheck
    bdestroy(bstr);
    mu_assert(strcmp("This is a test bstring.", testing_bstr->data) == 0, "assigned changes after source is destroyed.");

    bdestroy(testing_bstr);

    return NULL;
}

char *bassigncstr_test() {
    //set a bstring to a C string's contents
    char *str = "This is a test string.";
    bstring bstr = bfromcstr("starting value");

    mu_assert(strcmp(str, bstr->data) != 0, "strings are equal before bassigncstr");

    int rc = bassigncstr(bstr, str);
    mu_assert(rc == 0, "bassigncstr returned error");
    mu_assert(bstr != NULL, "bassigncstr changed bstr to NULL");
    mu_assert(strcmp(str, bstr->data) == 0, "strings arent equal after bassigncstr");

    bdestroy(bstr);

    return NULL;
}

char *bassignblk_test() {
    //set a bstring to a C string, but give the length
    char *str = "This is a test string.";
    int len_str = 22;
    bstring bstr = bfromcstr("starting value");

    mu_assert(strcmp(str, bstr->data) != 0, "string are equal before bassignblk");

    int rc = bassignblk(bstr, str, 0);
    mu_assert(rc == 0, "bassignblk returned error");
    mu_assert(bstr != NULL, "bassignblk changed bstr to NULL");
    mu_assert(strcmp("", bstr->data) == 0, "bstr should be empty after bassignblk with 0");

    rc = bassignblk(bstr, str, 4);
    mu_assert(rc == 0, "bassignblk returned error");
    mu_assert(bstr != NULL, "bassignblk changed bstr to NULL");
    mu_assert(strcmp("This", bstr->data) == 0, "bstr should be equal to 'This' after bassignblk with 4");

    rc = bassignblk(bstr, str, len_str);
    mu_assert(rc == 0, "bassignblk returned error");
    mu_assert(bstr != NULL, "bassignblk changed bstr to NULL");
    mu_assert(strcmp(str, bstr->data) == 0, "string arent equal after bassignblk with 22");

    bdestroy(bstr);

    return NULL;
}

char *bdestroy_test() {
    //destroy a bstring
    bstring bstr = bfromcstr("This is a test bstring.");

    int rc = bdestroy(bstr);
    mu_assert(rc == 0, "bdestroy returned error on valid call");

    //rc = bdestroy(bstr);
    //mu_assert(rc == -1, "bdestroy didnt return error on invalid call of already destroyed bstring");

    rc = bdestroy(NULL);
    mu_assert(rc == -1, "bdestroy didnt return error on invalid call of NULL");

    return NULL;
}

char *bconcat_test() {
    //concatinate one bstring to another
    bstring a = bfromcstr("abc");
    bstring b = bfromcstr("def");
    char *res = "abcdef";

    mu_assert(strcmp(res, a->data) != 0, "a equal to res before bconcat");

    int rc = bconcat(a, b);
    mu_assert(rc == 0, "bconcat returned error");
    mu_assert(strcmp(res, a->data) == 0, "a isnt equal to res after bconcat");
    mu_assert(strcmp("def", b->data) == 0, "b is changed after bconcat");

    bdestroy(a);
    bdestroy(b);

    return NULL;
}

char *bstricmp_test() {
    //compare two bstrings returning the same result as strcmp
    bstring a = bfromcstr("aaa");
    bstring b = bfromcstr("bbb");
    bstring c = bfromcstr("bbb");
    bstring d = bfromcstr("ccc");

    mu_assert(bstricmp(a, b) < 0, "bstricmp wrong on smaller");
    mu_assert(bstricmp(b, c) == 0, "bstricmp wrong on equal");
    mu_assert(bstricmp(c, d) < 0, "bstricmp wrong on bigger");

    mu_assert(strcmp("aaa", a->data) == 0, "a changed after bstricmp");
    mu_assert(strcmp("bbb", b->data) == 0, "b changed after bstricmp");
    mu_assert(strcmp("bbb", c->data) == 0, "c changed after bstricmp");
    mu_assert(strcmp("ccc", d->data) == 0, "d changed after bstricmp");

    bdestroy(a);
    bdestroy(b);
    bdestroy(c);
    bdestroy(d);

    return NULL;
}

char *biseq_test() {
    //test if two bstrings are equal
    bstring a = bfromcstr("aaa");
    bstring b = bfromcstr("bbb");
    bstring c = bfromcstr("bbb");
    bstring d = bfromcstr("bbbb");

    mu_assert(biseq(a, b) == 0, "biseq wrong on different");
    mu_assert(biseq(b, c) == 1, "biseq wrong on same");
    mu_assert(biseq(c, d) == 0, "biseq wrong on different lenght");
    mu_assert(biseq(c, NULL) == -1, "biseq wrong on error");

    mu_assert(strcmp("aaa", a->data) == 0, "a changed after bstricmp");
    mu_assert(strcmp("bbb", b->data) == 0, "b changed after bstricmp");
    mu_assert(strcmp("bbb", c->data) == 0, "c changed after bstricmp");
    mu_assert(strcmp("bbbb", d->data) == 0, "d changed after bstricmp");

    bdestroy(a);
    bdestroy(b);
    bdestroy(c);
    bdestroy(d);

    return NULL;
}

char *binstr_test() {
    //tell if one string is in another
    bstring bstr = bfromcstr("This is a test bstring.");
    bstring pattern1 = bfromcstr("is");
    bstring pattern2 = bfromcstr("bla");

    mu_assert(binstr(bstr, 0, pattern1) == 2, "binstr returned wrong index of first occurence");
    mu_assert(binstr(bstr, 3, pattern1) == 5, "binstr returned wrong index of second occurence when not starting from beginning");
    mu_assert(binstr(bstr, 10, pattern1) == -1, "binstr returned index when it shouldnt for pattern1");
    mu_assert(binstr(bstr, 0, pattern2) == -1, "binstr returned index when it shouldnt for pattern2");
    mu_assert(binstr(bstr, 0, NULL) == -1, "binstr returned index when it shouldnt for NULL");

    bdestroy(bstr);
    bdestroy(pattern1);
    bdestroy(pattern2);

    return NULL;
}

char *bfindreplace_test() {
    //find one bstring in another and replace with a third
    bstring bstr = bfromcstr("This is a test bstring.");
    bstring find = bfromcstr("is");
    bstring repl = bfromcstr("was");
    bstring res1 = bfromcstr("This is a test bstring.");
    bstring res2 = bfromcstr("Thwas was a test bstring.");
    bstring res3 = bfromcstr("This was a test bstring.");

    int rc = bfindreplace(bstr, find, repl, 10);
    mu_assert(rc == 0, "first bfindreplace returned error");
    mu_assert(bstricmp(res1, bstr) == 0, "wrong first result");

    rc = bfindreplace(bstr, find, repl, 0);
    mu_assert(rc == 0, "second bfindreplace returned error");
    mu_assert(bstricmp(res2, bstr) == 0, "wrong second result");

    bstr = bfromcstr("This is a test bstring.");

    rc = bfindreplace(bstr, find, repl, 3);
    mu_assert(rc == 0, "third bfindreplace returned error");
    mu_assert(bstricmp(res3, bstr) == 0, "wrong third result");

    //TODO, leakcheck
    bdestroy(bstr);
    bdestroy(find);
    bdestroy(repl);
    bdestroy(res1);
    bdestroy(res2);
    bdestroy(res3);

    return NULL;
}

char *bsplit_test() {
    //split a bstring into a bstrList
    bstring bstr = bfromcstr("This is a test bstring.");
    char *words[] = { "This", "is", "a", "test", "bstring." };

    struct bstrList *bstr_list = bsplit(bstr, ' ');
    for (int i=0; i<bstr_list->qty; i++) {
        mu_assert(strcmp(words[i], bstr_list->entry[i]->data)  == 0, "wrong parse");
    }

    bdestroy(bstr);
    bstrListDestroy(bstr_list);

    return NULL;
}

char *bformat_test() {
    //do a format string, which is super handy
    bstring bstr = bfromcstr("testing format int: 1, char: a, string: test");

    bstring res = bformat("testing format int: %d, char: %c, string: %s", 1, 'a', "test");
    mu_assert(biseq(bstr, res), "bformat failed");

    bdestroy(bstr);
    bdestroy(res);

    return NULL;
}

char *blength_test() {
    //get the lenght of bstring
    bstring bstr = bfromcstr("This is a test bstring.");
    //int bstr_len = 23;

    int len_bstr = blength(bstr);
    mu_assert(len_bstr == 23, "blength returned wrong len");

    bdestroy(bstr);

    return NULL;
}

char *bdata_test() {
    //get the date from a bstring
    char * str = "This is a test bstring.";
    bstring bstr = bfromcstr("This is a test bstring.");

    mu_assert(strcmp(str, bdata(bstr)) == 0, "bdata returned wrong string");

    bdestroy(bstr);

    return NULL;
}

char *bchar_test() {
    //get the char from a bstring
    bstring bstr = bfromcstr("This is a test bstring.");

    mu_assert(bchar(bstr, 0) == 'T', "wrong first char");
    mu_assert(bchar(bstr, 22) == '.', "wrong last char");
    mu_assert(bchar(bstr, 8) == 'a', "wrong middle char");

    bdestroy(bstr);

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

    //there are some leakchecks in this code
    //dont know if that is my fault of some functions here (bstrcpy, bassign, bfindreplace) dont dealoc correctly or dont allow for correct destroy/dealloc

    return NULL;
}

RUN_TESTS(all_test);
