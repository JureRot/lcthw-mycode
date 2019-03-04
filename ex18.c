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

// typedef creats a fake type
// in this case for a function pointer
typedef int (*compare_cb) (int a, int b);

/**
 * A classic bubble sort function that uses the
 * compare_cb to do the sorting.
 */
int* bubble_sort(int* numbers, int count, compare_cb cmp) {
    int temp = 0;
    //we reserve space for all numbers and copy the memory from *numbers to new reserved space
    //thus we create a copy of the numbers array
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
    //we go n-times over numbers
    //each time we take the biggest/smalles (depends on cmp) to the end
    return target;
    //we dont free target here, because the func that call this probably still needs it (if we free it it could be overwritten by than)
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
void test_sorting(int* numbers, int count, compare_cb cmp) {
    int* sorted = bubble_sort(numbers, count, cmp);
    if (!sorted)
        die("Failed to sort as requested");

    for (int i=0; i<count; i++) {
        printf("%d ", sorted[i]);
    }
    printf("\n");

    free(sorted);
    //here we free the target in bubble_sort
    //now i get it:
    //we do that becase in bubble_sort we create a copy of an array and we sort it
    //and we return the pointer to that sorted array
    //thats why we cant free it in bubble_sort, because we return the pointer to it
    //and the function that called bubble_sort probably still wants to use that sorted array

    //Breaking it
    unsigned char *data = (unsigned char *)cmp; //we cast the function code (as binary) to an unsiged char
    for (int i=0; i<25; i++) {
        printf("%02x:", data[i]);
    }
    printf("\n");
}

int main(int argc, char* argv[]) {
    if (argc < 2)
        die("USAGE: ex18 4 3 1 5 6"); 

    int count = argc - 1;
    char **inputs = argv + 1;
    //creates a pointer to the second element in argv (shiping ./ex18)
    //it think it could be done the same with char **inputs = &argv[1];

    int *numbers = malloc(count * sizeof(int));
    if (!numbers)
        die("Memory error");

    for (int i=0; i<count; i++) {
        numbers[i] = atoi(inputs[i]);
    }

    test_sorting(numbers, count, sorted_order);
    test_sorting(numbers, count, reverse_order);
    test_sorting(numbers, count, strange_order);
    //test_sorting(numbers, count, memcpy); //it complains if the func has the wrong signature
    //test_sorting(numbers, count, NULL); //it doesnt like NULL either

    free(numbers);
    
    return 0;
    /*
     * pointer to a function is just a pointer var that points to the location of the code of a specific function
     * we can assignt it a function eg. int (*tester) (int a, int b) = sorted_order;
     * this so now if we call tester(a, b) or if we call sorted_order(a, b) we would get the same thing.
     * we use the typdef so our new pointer to function becomes its own type and we can pass it any func as long as it matches the signature
     *
     * when we pass sorted_order to the test_sorting(), the compiler knows that the third argument should be of type compare_cb (because the typedef-ed it)
     * it knows that compare_cb is a pointer to a function and it sees that he was given a name of a function
     * and if the signature of the function it was given matches the signature of compare_cb type
     * it points to the location of the function it was given.
     */

    //TAKE A CLOSER LOOK AT THIS AND TRY TO UNDERSTAND IT COMPLETELLY (do some testing)
}

//char* str je enako kot char *str
//tole z * in kam jih postavs (zravn tipa (kar je logicno) al zravn imena) je nerodno
