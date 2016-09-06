#include <stdio.h>
#include <stdlib.h>
#include "io.h"
#include "data.h"

int
main ( int argc, char *argv[] )   {
	printf("Starting my job...\n");
    struct Person people[5];
    data(people, 5);
    qsort(people, 5, sizeof(struct Person), comp);
	io(people, 5);
	printf("Done with my job.\n");
        return(EXIT_SUCCESS);
}
