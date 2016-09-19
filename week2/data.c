#include <stdio.h>
#include "data.h"

void data(struct Person p[], int num_people) {
    int nth_person = 0;

    while (num_people-- > 0) {
        scanf("%d %1c", &p[nth_person].age, &p[nth_person].gender);
        nth_person++;
    }
};
