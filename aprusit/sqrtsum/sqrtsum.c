#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

double *sqrtsums;          // this data is shared by the thread(s)
void *runner(void *param); // threads call this function
struct threadargs {
	int index;
	int from;
	int to;
};

// The lowest N, for which the program will be run multithreaded.
//
// If N is very low, the overhead of running the sqrtsum function
// multithreaded will be significant, and will result in loss of performance.
// If N is very high, running the sqrtsum function in a single thread
// will perform worse than when run with multiple threads.
// We picked 100, it is a good number, it is also a decent hipshot.
#define MIN_N_MULTITHREADED 100

// calculates `sum sqrt(i), i=a to b`
double sqrt_sum_from_to(int a, int b);
// calculates `sum sqrt(i), i=1 to n`
double sqrt_sum_of(int n);

// calculates the sum of an array of doubles
double sum_of_doubles(double doubles[], int arraysize);

// prints the sum result to the console
void print_result(double result);

int main(int argc, char *argv[]) {
	pthread_attr_t attr;      // set of thread attributes
	pthread_attr_init(&attr); // get the default attributes

	int processorcount = sysconf(_SC_NPROCESSORS_ONLN);
	sqrtsums = malloc(sizeof(double) * processorcount);

	if (argc != 2) {
		fprintf(stderr, "usage: %s <integer value>\n", argv[0]);
		return -1;
	}
	if (atoi(argv[1]) < 0) {
		fprintf(stderr, "%d must be >= 0\n", atoi(argv[1]));
		return -1;
	}

	int N = atoi(argv[1]);
	if (N < MIN_N_MULTITHREADED) {
		// we do not want to go multithreaded on small N's,
		// see the comment at the definition of MIN_N_MULTITHREADED
		print_result(sqrt_sum_of(N));
		return 0;
	}

	pthread_t tid[processorcount]; // thread identifiers
	struct threadargs args[processorcount]; // args for the threads
	for (int i = 0; i < processorcount; i++) {
		// should also work for odd numbered processorcounts
		args[i] = (struct threadargs) {
			.index = i,
			// move multiplicative operands to the numerator to avoid
			// that the integer division cuts away too much information
			.from = (N * i) / processorcount + 1,
			.to = (N * (i+1)) / processorcount
		};

		// create the thread
		pthread_create(&tid[i], &attr, runner, &args[i]);
	}

	for (int i = 0; i < processorcount; i++) {
		pthread_join(tid[i], NULL);
	}

	print_result(sum_of_doubles(sqrtsums, processorcount));

	return 0;
}

void *runner(void *param) {
	struct threadargs *args = (struct threadargs*)param;
	sqrtsums[args->index] = 0;

	for (int i = args->from; i <= args->to; i++) {
		sqrtsums[args->index] += sqrt(i);
	}

	pthread_exit(0);
}

double sqrt_sum_of(int n) {
	return (sqrt_sum_from_to(1, n));
}

double sqrt_sum_from_to(int a, int b) {
	double sqrtsum = 0;
	for (int i = a; i <= b; i++) {
		sqrtsum += sqrt((double)i);
	}

	return sqrtsum;
}

double sum_of_doubles(double doubles[], int arraysize) {
	double sum = 0;
	for (int i = 0; i < arraysize; i++) {
		sum += doubles[i];
	}

	return sum;
}

void print_result(double result) {
	printf("sqrt sum = %.2f\n", result);
}
