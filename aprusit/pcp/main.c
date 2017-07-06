#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

#include "../list/list.h"

sem_t full;
sem_t empty;
List *buffer;
pthread_mutex_t product_lock;
int consumers;
int producers;
int buff_size;
int products;

/* Random Sleep function from assignment.*/
void Sleep(float wait_time_ms)
{
	wait_time_ms = ((float)rand())*wait_time_ms / (float)RAND_MAX;
	usleep((int) (wait_time_ms * 1e3f));
}

/* init_semaphores: Set full semaphore to 0 and empty semaphore to buffer size.*/
void init_semaphores(int buffer_size)
{
	sem_init(&full, 1, 0);
	sem_init(&empty, 1, buffer_size);
}

/* insert_item: Insert integer Item into the FIFO buffer.*/
void insert_item(int item)
{
	int* item_pointer = malloc(sizeof(int*));
	*item_pointer = item;
	list_add(buffer, node_new_int(item_pointer));
}

/*remove_item: Remove first item from the FIFO buffer.*/
void remove_item(int *item)
{
	Node *n = list_remove(buffer);
	*item = *(int*)(n->elm);
}

/*producer: Function run by producer threads. Will keep producing items,
			and inserting them in the buffer, as long as the buffer isn't full
			and it hasn't run out of products.*/
void* producer(void *arg)
{
	int id = *(int*)arg;
	while(1)
	{	
		//Lock the global variabel check and decrement.
		pthread_mutex_lock(&product_lock);
		if(products > 0)
		{
			products--;
			int item;
			
			//Produce products. The thread remains lock to avoid edge case pre-mature termination.
			item = rand() % 10000;
			sem_wait(&empty); //Wait for space to produce, if buffer is full.
			insert_item(item);
	
			//Record amount of products in full semaphore
			int buffer_val;
			sem_getvalue(&full, &buffer_val);
			pthread_mutex_unlock(&product_lock);
			
			//Print program state and signal full semaphore
			printf("Producer %d produced item %d. Items in buffer: %d out of %d.\n",
				id, item, buffer_val+1, buff_size);
			sem_post(&full);
			Sleep(500.0);
		} else {
			//If there are no products left, we still need to unlock the mutex.
			pthread_mutex_unlock(&product_lock);
			return NULL;
		}
	}
}

/*consumer: Function run by consumer threads. Will keep consuming items,
			removing them from the FIFO buffer, as long as it contains products.*/
void* consumer(void *arg)
{
	int id = *(int*)arg;
	while(1)
	{
		//Wait for there to be consumable items.
		sem_wait(&full);
		//Consume items
		int item;
		remove_item(&item);
		int buffer_val;
		
		//Signal empty semaphore and print thread status.
		sem_getvalue(&full, &buffer_val);
		printf("Consumer %d consumed item %d. Items in buffer: %d out of %d.\n",
			id, item, buffer_val, buff_size);
		sem_post(&empty);
		Sleep(500.0);
   }
}

/*main: Initiaites variables, and starts producer and consumer threads.*/
int main(int argc, char *argv[])
{
	setbuf(stdout,NULL);
	//Seeding random generator
	struct timeval tv;
	gettimeofday(&tv, NULL);
	srand(tv.tv_usec);

	if(argc != 5)
	{
		printf("Error: Wrong amount of parameters.\n");
		printf("Example usage: %s <n_producers> <n_comsumers> <buffer_size> <n_products>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	producers = atoi(argv[1]);
	printf("Producers: %d\n", producers);
	consumers = atoi(argv[2]);
	printf("Consumers: %d\n", consumers);
	buff_size = atoi(argv[3]);
	printf("Buffer size: %d\n", buff_size);
	products = atoi(argv[4]);
	printf("Products: %d\n", products);
	if( (producers < 1) || (consumers < 1) || (buff_size < 1) || (products < 1))
	{
		printf("Error: Non-positive input parameters.\n");
		exit(EXIT_FAILURE);
	}  

	//Initiating buffer
	buffer = list_new();

	//Initiating semaphores
	init_semaphores(buff_size);

	//Start producer and consumer threads.
	pthread_t producer_threads[producers];
	pthread_t consumer_threads[consumers];
	for(int index = 0; index < producers; ++index)
	{
		printf("Starting producer thread: %d\n", index);
		int *id = malloc(sizeof(int*));
		*id = index;
		pthread_create(&producer_threads[index], NULL, producer, (void*) id);
	}

	for(int index = 0; index < consumers; ++index)
	{
		printf("Starting consumer thread: %d\n", index);
		int *id = malloc(sizeof(int*));
		*id = index;
		pthread_create(&consumer_threads[index], NULL, consumer, (void*) id);
	}

	//Once every product is produced and the buffer is empty, the program terminates
	int full_val = 1;
	int empty_val = 0;
	while(products > 0 || full_val > 0 || empty_val < buff_size){
		Sleep(200.0);
		sem_getvalue(&full, &full_val);
		sem_getvalue(&empty, &empty_val);
	}
	printf("Exiting program.\n");
	return 0;
}

