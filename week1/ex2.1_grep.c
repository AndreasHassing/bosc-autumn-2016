/**
 * Andreas Bjørn Hassing Nielsen
 * Exercise 2.1: Write a tiny implementation of grep in C
 */

#include <stdio.h>
#include <string.h>

#define MAXBUF 512 /* length of largest string to read */

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: ./grep <filename> <search_string>\n");
        return 40;
    }

    FILE *file;
    file = fopen(argv[1], "r");

    int line_number = 1;
    char line[MAXBUF];

    while (fgets(line, MAXBUF, file)) {
        if (strstr(line, argv[2])) {
            printf("%d: %s", line_number, line);
        }

        line_number = line_number + 1;
    }

    fclose(file);

    return 0;
}
