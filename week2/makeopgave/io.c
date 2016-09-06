#include <stdio.h>
#include "io.h"

void io(struct Person p[], int num_people) {
    int i;
    for (i = 0; i < num_people; i++) {
        printf("Person %d, age: %d and gender: %c\n", i, p[i].age, p[i].gender);
    }
};
