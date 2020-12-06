#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

int* arr;
sem_t lock;
sem_t can_add;
sem_t can_sub;

void seed() {
	struct timespec now;
	if (clock_gettime(CLOCK_REALTIME, &now) < 0) {
		fprintf(stderr, "Error with time function!\n");
		exit(EXIT_FAILURE);
	}
	srand(now.tv_nsec);
}

void * producerFunc (void * arg) {

	while (1)
	{
		int value = rand() % 99 + 1;
		int index;
		sem_wait(&can_add);
		sem_wait(&lock);
		sem_getvalue(&can_add, &index);
		arr[index] = value;
		// printf("\tIncrement thread: (%u), iteration: %d\n", (unsigned int)pthread_self(), i + 1);
		sem_post(&lock);
		sem_post(&can_sub);
		sleep(1);
	}

	return NULL;
}

void * consumerFunc (void * arg) {

	while (1)
	{
		int index;
		int getValueArray;
		sem_wait(&can_sub);
		sem_wait(&lock);
		sem_getvalue(&can_add, &index);
		getValueArray = arr[index];
		sem_post(&lock);
		sem_post(&can_add);
		printf("thread: (%u), value: %d\n", (unsigned int)pthread_self(), getValueArray);
		sleep(1);
	}

	return NULL;
}

int main(int argc, char const *argv[]) {
	int sizeElem = 10;
	int countPair = 2;
	
	pthread_t* tidProd = (pthread_t*)malloc(countPair * sizeof(pthread_t));
	pthread_t* tidCons = (pthread_t*)malloc(countPair * sizeof(pthread_t));
	

	setbuf(stdout, NULL);

	if (argc == 2)
	{
		sizeElem = atoi(argv[1]);
	}
	else
	{
		fprintf(stderr, "Cant receive value\n" );
		// return EXIT_FAILURE;
	}
	sem_init(&lock, 0, 1);
	sem_init(&can_add, 0, sizeElem);
	sem_init(&can_sub, 0, 0);
	arr = (int*)malloc(sizeElem * sizeof(int));

	// printf("Before. val = %ld\n", val);

	seed();
	for (int i = 0; i < countPair; ++i)
	{
		if (pthread_create(&tidProd[i], NULL, &producerFunc, NULL) != 0)
		{
			fprintf(stderr, "Error pthread_create()\n");
		}
		if (pthread_create(&tidCons[i], NULL, &consumerFunc, NULL) != 0)
		{
			fprintf(stderr, "Error pthread_create()\n");
		}
	}
	sleep(5);
	for (int i = 0; i < countPair; ++i)
	{
		if (pthread_cancel(tidProd[i]) != 0)
		{
			fprintf(stderr, "Cant cancel thread\n" );
			sleep(3);
			return EXIT_FAILURE;
		}
		if (pthread_cancel(tidCons[i]) != 0)
		{
			fprintf(stderr, "Cant cancel thread\n" );
			sleep(3);
			return EXIT_FAILURE;
		}
	}

	for (int i = 0; i < countPair; ++i)
	{
		if (pthread_join(tidProd[i], NULL) != 0)
		{
			fprintf(stderr, "Join error\n");
		}
		if (pthread_join(tidCons[i], NULL) != 0)
		{
			fprintf(stderr, "Join error\n");
		}
	}


	
	free(tidProd);
	free(tidCons);
	free(arr);
	sem_destroy(&lock);
	sem_destroy(&can_add);
	sem_destroy(&can_sub);

	return EXIT_SUCCESS;
}