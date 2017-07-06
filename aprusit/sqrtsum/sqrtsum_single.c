#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	if (argc != 2) {
		fprintf(stderr, "usage: %s <integer value>\n", argv[0]);
		return -1;
	}
	if (atoi(argv[1]) < 0) {
		fprintf(stderr, "%d must be >= 0\n", atoi(argv[1]));
		return -1;
	}

	double sqrtsum = 0;
	for (int i = 0; i <= atoi(argv[1]); i++) {
		sqrtsum += sqrt(i);
	}

	printf("sqrt sum = %.2f\n", sqrtsum);
}
