#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h> // for usleep
#include <string.h> // for memcpy
#include <pthread.h>

#include "array_helpers.h"

typedef struct state
{
	int *resource;
	int *available;
	int **max;
	int **allocation;
	int **need;
} State;

// Global variables
int process_count, resource_count;
State *s = NULL;

// Mutex for access to state.
pthread_mutex_t state_mutex;

// Run the Safety Algorithm on the state `s`
int is_safe() {
	int safe = 1; // assume the state is safe, then try to disprove it

	int *work = malloc(resource_count * sizeof(int));
	memcpy(work, s->available, resource_count * sizeof(int));
	// use calloc for `finish`, to initialize elements
	// in the array to `0` which is also `False`
	int *finish = calloc(process_count, sizeof(int));

	for (int i = 0; i < process_count; i++) {
		if (!finish[i] && array_lte(s->need[i], work, resource_count)) {
			array_add(work, s->allocation[i], resource_count);
			finish[i] = 1;
			i = -1; // reset the index to start from 0 on the next run
		}
	}

	for (int i = 0; i < process_count; i++) {
		if (!finish[i]) { safe = 0; break; }
	}

	free(work);
	free(finish);

	return safe;
}

/* Random sleep function */
void Sleep(float wait_time_ms)
{
	// add randomness
	wait_time_ms = ((float)rand()) * wait_time_ms / (float)RAND_MAX;
	usleep((int)(wait_time_ms * 1e3f)); // convert from ms to us
}

// Run a request against the state, deducting
// the request from available resources, adding
// the request to allocation for the process and
// deducting the request from the needed resources
// of the process.
void run_request(int i, int *request) {
	array_subtract(s->available, request, resource_count);
	array_add(s->allocation[i], request, resource_count);
	array_subtract(s->need[i], request, resource_count);
}

// Reverts a request. See the documentation for `run_request`,
// and revert the process horizontally and vertically.
void revert_request(int i, int *request) {
	array_add(s->need[i], request, resource_count);
	array_subtract(s->allocation[i], request, resource_count);
	array_add(s->available, request, resource_count);
}

/* Allocate resources in request for process i, only if it
   results in a safe state and return 1, else return 0 */
int resource_request(int i, int *request)
{
	if (!array_lte(request, s->need[i], resource_count)) {
		printf("Process %d has exceeded its maximum claim.\n", i);
		exit(EXIT_FAILURE);
	}
	pthread_mutex_lock(&state_mutex);
	if (!array_lte(request, s->available, resource_count)) {
		pthread_mutex_unlock(&state_mutex);
		return 0; // we can't request those resources yet!
	}

	run_request(i, request);

	int state_is_safe = is_safe();

	if (!state_is_safe) {
		revert_request(i, request);
#ifdef DEBUG
		printf("Process %d: request is unsafe.\n  request: [%d %d %d]\n",
		    i, request[0], request[1], request[2]);
#endif
	}

	pthread_mutex_unlock(&state_mutex);
	return state_is_safe;
}

/* Release the resources in request for process i */
void resource_release(int i, int *request)
{
	pthread_mutex_lock(&state_mutex);
	revert_request(i, request);
	pthread_mutex_unlock(&state_mutex);
}

/* Generate a request vector */
void generate_request(int i, int *request)
{
	int j, sum = 0;
	while (!sum)
	{
		for (j = 0; j < resource_count; j++)
		{
			request[j] = rand() % (s->need[i][j] + 1);
			sum += request[j];
		}
	}
	printf("Process %d: Requesting resources.\n", i);
}

/* Generate a release vector */
void generate_release(int i, int *request)
{
	int j, sum = 0;
	while (!sum)
	{
		for (j = 0; j < resource_count; j++)
		{
			request[j] = rand() % (s->allocation[i][j] + 1);
			sum += request[j];
		}
	}
	printf("Process %d: Releasing resources.\n", i);
}

/* Threads starts here */
void *process_thread(void *param)
{
	/* Process number */
	int i = (int)(long)param;
	/* Allocate request vector */
	int *request = malloc(resource_count * sizeof(int));
	while (1)
	{
		/* Generate request */
		generate_request(i, request);
		while (!resource_request(i, request))
		{
			/* Wait */
			Sleep(100);
		}
		/* Generate release */
		generate_release(i, request);
		/* Release resources */
		resource_release(i, request);
		/* Wait */
		Sleep(1000);
	}
	free(request);
}

// Allocate resources to the state structure.
void allocate_state() {
	s = (State *) malloc(sizeof(State));
	s->resource = malloc(resource_count * sizeof(int));
	s->available = malloc(resource_count * sizeof(int));

	s->max = malloc(process_count * sizeof(int*));
	s->allocation = malloc(process_count * sizeof(int*));
	s->need = malloc(process_count * sizeof(int*));
	for (int i = 0; i < process_count; i++) {
		s->max[i] = malloc(resource_count * sizeof(int));
		s->allocation[i] = malloc(resource_count * sizeof(int));
		s->need[i] = malloc(resource_count * sizeof(int));
	}
}

// Free dynamically allocated resources from
// the State structure and its members. Then
// NULL the State variable `s`.
void free_state() {
	for (int i = 0; i < process_count; i++) {
		free(s->max[i]);
		free(s->allocation[i]);
		free(s->need[i]);
	}
	free(s->max);
	free(s->allocation);
	free(s->need);

	free(s->resource);
	free(s->available);
	free(s);
	s = NULL;
}

int main(int argc, char *argv[])
{
	/* Get size of current state as input */
	int i, j;
	printf("Number of processes: ");
	scanf("%d", &process_count); // n
	printf("Number of resources: ");
	scanf("%d", &resource_count); // m

	/* Allocate memory for state */
	if (s == NULL)
	{
		allocate_state();
	};

	/* Get current state as input */
	printf("Resource vector: ");
	for (i = 0; i < resource_count; i++)
		scanf("%d", &s->resource[i]);
	printf("Enter max matrix: ");
	for (i = 0; i < process_count; i++)
		for (j = 0; j < resource_count; j++)
			scanf("%d", &s->max[i][j]);
	printf("Enter allocation matrix: ");
	for (i = 0; i < process_count; i++)
		for (j = 0; j < resource_count; j++)
			scanf("%d", &s->allocation[i][j]);
	printf("\n");

	/* Calcuate the need matrix */
	for (i = 0; i < process_count; i++) {
		for (j = 0; j < resource_count; j++) {
			s->need[i][j] = s->max[i][j] - s->allocation[i][j];
			if (s->need[i][j] < 0) {
				printf("Too many resources of type %d are allocated for process %d.\n", j, i);
				exit(EXIT_FAILURE);
			}
		}
	}

	/* Calcuate the availability vector */
	for (j = 0; j < resource_count; j++)
	{
		int sum = 0;
		for (i = 0; i < process_count; i++)
			sum += s->allocation[i][j];
		s->available[j] = s->resource[j] - sum;
	}

	/* Output need matrix and availability vector */
	printf("Need matrix:\n");
	for (i = 0; i < resource_count; i++)
		printf("R%d ", i + 1);
	printf("\n");
	for (i = 0; i < process_count; i++)
	{
		print_array("%d  ", s->need[i], resource_count);
		printf("\n");
	}
	printf("Availability vector:\n");
	for (i = 0; i < resource_count; i++)
		printf("R%d ", i + 1);
	printf("\n");
	print_array("%d  ", s->available, resource_count);
	printf("\n");

	/* If initial state is unsafe then terminate with error */
	if (!is_safe()) {
		fprintf(stderr, "The initial state is unsafe!\n");
		exit(EXIT_FAILURE);
	}
	printf("The initial state is safe!\n");

	/* Seed the random number generator */
	struct timeval tv;
	gettimeofday(&tv, NULL);
	srand(tv.tv_usec);

	/* Create process_count threads */
	pthread_t *tid = malloc(process_count * sizeof(pthread_t));
	for (i = 0; i < process_count; i++)
		pthread_create(&tid[i], NULL, process_thread, (void *)(long)i);

	/* Wait for threads to finish */
	pthread_exit(0);
	free(tid);

	/* Free state memory */
	free_state();
}
