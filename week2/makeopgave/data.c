#include <stdio.h>
#include "data.h"

void data(struct Person p[], int num_people) {
    int i = 0;

    while (num_people-- > 0) {
        printf("Enter person %d's gender: ", i+1);
        scanf(" %1c", &p[i].gender);
        printf("Enter persons %d's age: ", i+1);
        scanf("%d", &p[i].age);
        i++;
        printf("\n");
    }

    printf("Done.\n");
};
