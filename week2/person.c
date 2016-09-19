#include "person.h"

int comp(const void * p1, const void * p2) {
    struct Person *pp1 = (struct Person *) p1;
    struct Person *pp2 = (struct Person *) p2;

    int age1 = pp1->age;
    int age2 = pp2->age;

    if (age1 > age2) return  1;
    if (age1 < age2) return -1;
    return 0;
}
