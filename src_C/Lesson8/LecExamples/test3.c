#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

long val;
sem_t lock;
sem_t can_add;
sem_t can_sub;

void * increment(void * arg) {
	int num = *(int*) arg;
	int i;

	for (i = 0; i < num; i++) {
		sem_wait(&can_add);
		sem_wait(&lock);
		val += 1;
		printf("\tIncrement thread: (%u), iteration: %d\n", (unsigned int)pthread_self(), i+1);
		sem_post(&lock);
		sem_post(&can_sub);
	}

	return NULL;
}

void * decrement(void * arg) {
	int num = *(int*) arg;
	int i;

	for (i = 0; i < num; i++) {
		sem_wait(&can_sub);
		sem_wait(&lock);
		val -= 1;
		printf("\t\tDecrement thread: (%u), iteration: %d\n", (unsigned int)pthread_self(), i+1);
		sem_post(&lock);
		sem_post(&can_add);
	}

	return NULL;
}

int main(void) {
	int num = 100000;
	pthread_t tid1, tid2;
	setbuf(stdout, NULL);

	val = 0;
	sem_init(&lock, 0, 1);
	sem_init(&can_add, 0, 1);
	sem_init(&can_sub, 0, 0);

	printf("Before. val = %ld\n", val);
	

	pthread_create(&tid1, NULL, increment, &num);
	pthread_create(&tid2, NULL, decrement, &num);
	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);

	printf("After. val = %ld\n", val);

	sem_destroy(&lock);
	sem_destroy(&can_add);
	sem_destroy(&can_sub);

	return EXIT_SUCCESS;
}