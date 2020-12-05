#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

int* arr;
sem_t lock;
sem_t can_add;
sem_t can_sub;

void * producerFunc (void * arg) {
	int num = *(int*) arg;
	int i;

	while (1) {
		sem_wait(&can_add);
		sem_wait(&lock);
		val += 1;
		printf("\tIncrement thread: (%u), iteration: %d\n", (unsigned int)pthread_self(), i + 1);
		sem_post(&lock);
		sem_post(&can_sub);
	}

	return NULL;
}

void * consumerFunc (void * arg) {
	int num = *(int*) arg;
	int i;

	for (i = 0; i < num; i++) {
		sem_wait(&can_sub);
		sem_wait(&lock);
		val -= 1;
		printf("\t\tDecrement thread: (%u), iteration: %d\n", (unsigned int)pthread_self(), i + 1);
		sem_post(&lock);
		sem_post(&can_add);
	}

	return NULL;
}

int main(int argc, char const *argv[]) {
	int size = 10;
	pthread_t tid1, tid2;
	setbuf(stdout, NULL);

	if (argc == 2)
	{
		size = atoi(argv[1]);
	}
	else
	{
		fprintf(stderr, "Cant receive value\n" );
		return EXIT_FAILURE;
	}
	sem_init(&lock, 0, 1);
	sem_init(&can_add, 0, 1);
	sem_init(&can_sub, 0, 0);

	printf("Before. val = %ld\n", val);


	if (pthread_create(&tid1, NULL, increment, &num) != 0)
	{
		fprintf(stderr, "Error pthread_create()\n");
	}
	if (pthread_create(&tid2, NULL, decrement, &num) != 0)
	{
		fprintf(stderr, "Error pthread_create()\n");
	}

	sleep(15);

	if (pthread_cancel(tid1) != 0)
	{
		fprintf(stderr, "Cant cancel thread\n" );
		sleep(3);
		pthread_cancel(tid1);
		return EXIT_FAILURE;
	}
	if (pthread_cancel(tid2) != 0)
	{
		fprintf(stderr, "Cant cancel thread\n" );
		sleep(3);
		pthread_cancel(tid2);
		return EXIT_FAILURE;
	}

	if (pthread_join(tid1, NULL) != 0)
	{
		fprintf(stderr, "Join error\n");
	}
	if (pthread_join(tid2, NULL) != 0)
	{
		fprintf(stderr, "Join error\n");
	}


	sem_destroy(&lock);
	sem_destroy(&can_add);
	sem_destroy(&can_sub);

	return EXIT_SUCCESS;
}