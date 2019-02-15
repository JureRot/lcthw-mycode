#include <stdio.h>

int main(int argc, char *argv[]) {
    //OK, THIS IS WEIRD, NOT THE SAME AS IN VIDEO LECTURE
    
    //go through each string in argv
    //why am I skipping arg[0]?
    for (int i=1; i<argc; i++) {
        printf("arg %d: %s\n", i, argv[i]);
    }

    //let's make our own array of strings
    char *states[] = {
        "California", "Oregon",
        "Washington", "Texas"
    };

    int num_states = 4;

    for (int i=0; i<num_states; i++) {
        printf("state %d: %s\n", i, states[i]);
    }

    return 0;
}
