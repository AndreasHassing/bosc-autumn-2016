/**
 * Andreas Bjørn Hassing Nielsen
 * Exercise 1.2: Write a program in C that takes arguments
 *               from the command line
 */

#include <stdio.h>

int main(int argc, char *argv[]) {
    // in C, the program name is put into argv[0]
    if (argc < 2) {
        printf("You need to provide an argument for this program to print.\n");
        return -1;
    }

    printf("%s\n", argv[1]);

    return 0;
}
