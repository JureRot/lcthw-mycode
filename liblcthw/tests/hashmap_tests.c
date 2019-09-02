#include "minunit.h"
#include <lcthw/hashmap.h>
//#include "../src/lcthw/hashmap.h" //just for writing
#include <assert.h>
#include <lcthw/bstrlib.h>
//#include "../src/lcthw/bstrlib.h" //just for writing

Hashmap *map = NULL;
static int  traverse_called = 0;
struct tagbstring test1 = bsStatic("test data 1");
struct tagbstring test2 = bsStatic("test data 2");
struct tagbstring test3 = bsStatic("test data 3");
struct tagbstring expect1 = bsStatic("THE VALUE 1");
struct tagbstring expect2 = bsStatic("THE VALUE 2");
struct tagbstring expect3 = bsStatic("THE VALUE 3");

struct tagbstring key1 = bsStatic("1");
struct tagbstring value1 = bsStatic("one");
struct tagbstring key2 = bsStatic("2");
struct tagbstring value2 = bsStatic("two");
struct tagbstring key3 = bsStatic("3");
struct tagbstring value3 = bsStatic("three");
struct tagbstring key4 = bsStatic("4");
struct tagbstring value4 = bsStatic("four");
struct tagbstring key5 = bsStatic("5");
struct tagbstring value5 = bsStatic("five");
struct tagbstring key6 = bsStatic("6");
struct tagbstring value6 = bsStatic("six");
struct tagbstring key7 = bsStatic("7");
struct tagbstring value7 = bsStatic("seven");
struct tagbstring key8 = bsStatic("8");
struct tagbstring value8 = bsStatic("eight");
struct tagbstring key9 = bsStatic("9");
struct tagbstring value9 = bsStatic("nine");
struct tagbstring key10 = bsStatic("10");
struct tagbstring value10 = bsStatic("ten");
struct tagbstring value10two = bsStatic("ten2");

//traverse function that increments the traverse_called (counts number of nodes)
static int traverse_good_cb(HashmapNode *node) {
    debug("KEY: %s", bdata((bstring) node->key));
    traverse_called++;
    return 0;
}

//same as traverse_good_cb, but fails on the third node on purpose
static int traverse_fail_cb(HashmapNode *node) {
    debug("KEY: %s", bdata((bstring) node->key));
    traverse_called++;

    if (traverse_called == 2) {
        return 1;
    } else {
        return 0;
    }
}

//traverse for human_test()
static int traverse_human_test(HashmapNode *node) {
	printf("node addr: %p, bucket: %d, key: %s, value: %s\n", node, node->hash%100, bdata((bstring)node->key), bdata((bstring)node->data));

	return 0;
}

char *test_human_test() { //test for humans to understand the workings
    map = Hashmap_create(NULL, NULL);

	int rc = Hashmap_set(map, &key1, &value1);
	rc = Hashmap_set(map, &key2, &value2);
	rc = Hashmap_set(map, &key3, &value3);
	rc = Hashmap_set(map, &key4, &value4);
	rc = Hashmap_set(map, &key5, &value5);
	rc = Hashmap_set(map, &key6, &value6);
	rc = Hashmap_set(map, &key7, &value7);
	rc = Hashmap_set(map, &key8, &value8);
	rc = Hashmap_set(map, &key9, &value9);
	rc = Hashmap_set(map, &key10, &value10);
	rc = Hashmap_set(map, &key10, &value10two);

	Hashmap_traverse(map, traverse_human_test);

    Hashmap_destroy(map);

	return NULL;
}

char *test_create() {
    map = Hashmap_create(NULL, NULL);
    mu_assert(map != NULL, "Failed to create map.");

    return NULL;
}

char *test_destroy() {
    Hashmap_destroy(map);

    return NULL;
}

char *test_get_set() {
    int rc = Hashmap_set(map, &test1, &expect1);
    mu_assert(rc == 0, "Failed to set &test1");
    bstring result = Hashmap_get(map, &test1);
    mu_assert(result == &expect1, "Wrong value for test1");

    rc = Hashmap_set(map, &test2, &expect2);
    mu_assert(rc == 0, "Failed to set &test2");
    result = Hashmap_get(map, &test2);
    mu_assert(result == &expect2, "Wrong value for test2");

    rc = Hashmap_set(map, &test3, &expect3);
    mu_assert(rc == 0, "Failed to set &test3");
    result = Hashmap_get(map, &test3);
    mu_assert(result == &expect3, "Wrong value for test3");

    return NULL;
}

char *test_traverse() {
    int rc = Hashmap_traverse(map, traverse_good_cb);
    mu_assert(rc == 0, "Failed the traverse.");
    mu_assert(traverse_called == 3, "Wrong count traverse.");

    traverse_called = 0;
    rc = Hashmap_traverse(map, traverse_fail_cb);
    mu_assert(rc == 1, "Failed the traverse.");
    mu_assert(traverse_called == 2, "Wrong count traverse for fail.");

    return NULL;
}

char *test_delete() {
    bstring deleted = (bstring) Hashmap_delete(map, &test1);
    mu_assert(deleted != NULL, "Got NULL on delete.");
    mu_assert(deleted == &expect1, "Should get test1");
    bstring result = Hashmap_get(map, &test1);
    mu_assert(result == NULL, "Should delete.");

    deleted = (bstring) Hashmap_delete(map, &test2);
    mu_assert(deleted != NULL, "Got NULL on delete.");
    mu_assert(deleted == &expect2, "Should get test2");
    result = Hashmap_get(map, &test2);
    mu_assert(result == NULL, "Should delete.");

    deleted = (bstring) Hashmap_delete(map, &test3);
    mu_assert(deleted != NULL, "Got NULL on delete.");
    mu_assert(deleted == &expect3, "Should get test3");
    result = Hashmap_get(map, &test3);
    mu_assert(result == NULL, "Should delete.");

    return NULL;
}

char *all_tests() {
    mu_suite_start();

	mu_run_test(test_human_test);

    mu_run_test(test_create);
    mu_run_test(test_get_set);
    mu_run_test(test_traverse);
    mu_run_test(test_delete);
    mu_run_test(test_destroy);

    return NULL;
}

RUN_TESTS(all_tests);
