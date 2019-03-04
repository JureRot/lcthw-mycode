#include <stdio.h>
#include <assert.h> //to get assert()
#include <stdlib.h> //to get malloc()
#include <string.h> //to get strdup()

struct Person {
    char *name;
    int age;
    int height;
    int weight;
};

struct Person *Person_create(char *name, int age, int height, int weight) { //gets argumetns and returns pointer of type struct Person
    struct Person *who = malloc(sizeof(struct Person));
    //reserves a chunk of memory the size of Person struct and gives us the pointer to its location
    assert(who != NULL);
    //asserts whether the condition is true, if false it fails/stops/Assertion failed:
    //here we are checking if OS gave us the memory

    who->name = strdup(name);
    //string duplicate, reserves new peace of memory and fills it with a copy of original string and returns us the pointer to it
    //this means we have another piece of memory inside of this piece of memory (this struct) (will be important when freeing the memory)
    who->age = age;
    who->height = height;
    who->weight = weight;

    return who;
    //and return the pointer to this struct (piece of memory)
}

void Person_destroy(struct Person *who) { //this is manual garbage collection
    assert(who != NULL);
    //again, if false, we stop right here
   
    free(who->name);
    //this is that name string that we need to free (because it is used by the struct / struct has the pointer to is)
    free(who);

    //freeing the memory doesnt erase it, we just take it out of the list of memory locations that this program has controll over
    //this just means this memory can now be allocated to something other
    //the pointer to who still exists and still points to this location
    //and before something writes over this memory, this struct will still hold its values and be accessable
    //but none of this is secure
}

void Person_print(struct Person *who) {
    printf("Name: %s\n", who->name);
    printf("\tAge: %d\n", who->age);
    printf("\tHeight: %d\n", who->height);
    printf("\tWeight: %d\n", who->weight);
}

int main (int argc, char *argv[]) {

    //make two people structures
    struct Person *joe = Person_create("Joe Alex", 32, 64, 140);
    struct Person *frank = Person_create("Frank Blank", 20, 72, 180);

    //print them out and where they are in memory
    printf("Joe is at memory location: %p\n", joe);
    Person_print(joe);

    printf("Frank is at memory location: %p\n", frank);
    Person_print(frank);

    //make everyone age 20 years and print them again
    joe->age += 20;
    joe->height -= 2;
    joe->weight += 40;
    Person_print(joe);

    frank->age += 20;
    frank->weight += 20;
    Person_print(frank);
    
    //destroy them both so we clean up
    Person_destroy(joe);
    Person_destroy(frank);
    //use valgrind to detect memory leaks (hope vscode can do the same)

    return 0;
}
