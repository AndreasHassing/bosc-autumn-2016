/******************************************************************************
main.c

Implementation of a simple FIFO buffer as a linked list defined in list.h.

******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "list.h"

#define N_THREADS 2

// FIFO list;
List *fifo;

// Threads
pthread_t tid[N_THREADS];

/* Test the synchronization of threads, using sleep
to avoid one thread to finish its job instantly */
void* test(void* args) {
	/* Convert thread number to char */
	int i = (int)(long)args;
	char tid[3] = {'a', i+'0', '\0'}; //"String"

	sleep(0.5);
	list_add(fifo, node_new_str(tid));
	sleep(0.5);
	print_structure(fifo);
	sleep(0.5);
	tid[0] = 'b';
	list_add(fifo, node_new_str(tid));
	sleep(0.5);
	print_structure(fifo);
	sleep(0.5);
	tid[0] = 'c';
	list_add(fifo, node_new_str(tid));
	sleep(0.5);
	print_structure(fifo);
	sleep(0.5);
	list_remove(fifo);
	sleep(0.5);
	print_structure(fifo);
	sleep(0.5);
	tid[0] = 'd';
	list_add(fifo, node_new_str(tid));
	sleep(0.5);
	print_structure(fifo);
	sleep(0.5);
	list_remove(fifo);
	sleep(0.5);
	print_structure(fifo);
}

int main(int argc, char* argv[])
{
	fifo = list_new();

	/* Start worker threads */
	int i = 0;
	while (i < N_THREADS) {
		pthread_create(&(tid[i]), NULL, &test, (void *)(long)++i);
	}

	/* Wait for threads to finish work */
	int j = 0;
	while (j < N_THREADS) {
		pthread_join(tid[j], NULL);
		j++;
	}

	printf("List length: %i\nFinal structure:\n", fifo->len);
	print_structure(fifo);

	/* Clean up */
	list_delete(fifo);

	return 0;
}
