#include <stdio.h>
#include "io.h"

void
io(struct Person p[], int num_people) {
    int i;
    for (i = 0; i < num_people; i++) {
        printf("Person age: %d and gender: %c\n", p[i].age, p[i].gender);
    }
};
