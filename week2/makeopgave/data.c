#include <stdio.h>
#include "data.h"

void data(struct Person p[], int num_people) {
    int nth_person = 1;

    while (num_people-- > 0) {
        printf("Enter person %d's gender: ", nth_person);
        scanf(" %1c", &p[nth_person-1].gender);
        printf("Enter persons %d's age: ", nth_person);
        scanf("%d", &p[nth_person-1].age);
        nth_person++;
        printf("\n");
    }
};
