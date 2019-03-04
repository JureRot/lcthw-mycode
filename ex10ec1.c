#include <stdio.h>
#include <ctype.h> //to inport toupper() and isupper()
#include <string.h> //to import strlen()

int main (int argc, char *argv[]) {
    if (argc != 2) {
        printf("ERROR: You need one argument.\n");
        //this is how you abort a program
        return 1;
    }

    for (int i=0; argv[1][i]!='\0'; i++) {
       char letter = argv[1][i];

       switch (letter) { //change all vowels to uppercase
           case 'a':
               argv[1][i] = toupper(letter);
               break;
           case 'e':
               argv[1][i] = toupper(letter);
               break;

           case 'i':
               argv[1][i] = toupper(letter);
               break;

           case 'o':
               argv[1][i] = toupper(letter);
               break;

           case 'u':
               argv[1][i] = toupper(letter);
               break;

           case 'y':
               if (i > 2) {
                   // it's only sometimes Y
                   argv[1][i] = toupper(letter);
               }
               break;
       }
    }
    
    //print only the uppercase letters
    for (int i=0; i<strlen(argv[1]); i++) { 
        if (isupper(argv[1][i])) {
            printf("%c", argv[1][i]);
        }
    }
    printf("\n");

    return 0;
}
