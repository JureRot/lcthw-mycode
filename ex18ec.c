#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

/** Our old friend from ex17 */
void die(const char* message) {
    if (errno) {
        perror(message);
    } else {
        printf("ERROR: %s\n", message);
    }
    exit(1);
}

typedef int (*compare_cb) (int a, int b);

//typedef for func pointer for sorting algorithm
typedef int *(*sort_alg) (int*, int, compare_cb);

/**
 * A classic bubble sort function that uses the
 * compare_cb to do the sorting.
 */
int* bubble_sort(int* numbers, int count, compare_cb cmp) {
    int temp = 0;
    int* target = malloc(count * sizeof(int));

    if (!target)
        die("Memory error");

    memcpy(target, numbers, count * sizeof(int));

    for (int i=0; i<count; i++) {
        for (int j=0; j<count-1; j++) {
            if (cmp(target[j], target[j+1]) > 0) {
                temp = target[j+1];
                target[j+1] = target[j];
                target[j] = temp;
            }
        }
    }
    return target;
}

int *selection_sort(int *numbers, int count, compare_cb cmp) {
    int temp = 0;
    //we make a copy of numbers (NEEDS TO BE FREED LATER)
    int *target = malloc(count * sizeof(int));
    if (!target)
        die("Memory error");
    memcpy(target, numbers, count * sizeof(int));

    for (int i=0; i<count-1; i++) {
        int best = i;
        for (int j=i+1; j<count; j++) {
            if (cmp(target[j], target[best]) < 0) {
                best = j;
            }
        }
        temp = target[i];
        target[i] = target[best];
        target[best] = temp;
    }
    //we go from the first position till the end and find the smallest/biggest (depends on cmp)
    //we swap the smallest/biggest with the one in the first position
    //we go from the second position till the end and find the (second) smallest/biggest
    //we swap the (second) smallest/biggest with the one in the second position
    //...
    return target;
}

int sorted_order(int a, int b) {
    return a-b;
}

int reverse_order(int a, int b) {
    return b-a;
}

int strange_order(int a, int b) {
    if (a==0 || b==0) {
        return 0;
    } else {
        return a%b;
    }
}

/**
 * Used to test that we are sorting thing correctly
 * by doing the sort and printing it out.
 */
void test_sorting(int* numbers, int count, sort_alg sort, compare_cb cmp) {
    int* sorted = sort(numbers, count, cmp);
    if (!sorted)
        die("Failed to sort as requested");

    for (int i=0; i<count; i++) {
        printf("%d ", sorted[i]);
    }
    printf("\n");

    free(sorted);
}

int main(int argc, char* argv[]) {
    if (argc < 2)
        die("USAGE: ex18 4 3 1 5 6"); 

    int count = argc - 1;
    char **inputs = argv + 1;

    int *numbers = malloc(count * sizeof(int));
    if (!numbers)
        die("Memory error");

    for (int i=0; i<count; i++) {
        numbers[i] = atoi(inputs[i]);
    }

    printf("bubble_sort:\n");
    test_sorting(numbers, count, bubble_sort, sorted_order);
    test_sorting(numbers, count, bubble_sort, reverse_order);
    test_sorting(numbers, count, bubble_sort, strange_order);

    printf("\n");

    printf("selection_sort:\n");
    test_sorting(numbers, count, selection_sort, sorted_order);
    test_sorting(numbers, count, selection_sort, reverse_order);
    test_sorting(numbers, count, selection_sort, strange_order);

    free(numbers);
    
    return 0;
}
