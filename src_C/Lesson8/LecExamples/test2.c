#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

sem_t s;

void * child(void * arg) {
	printf("\tchild : work\n");
	sem_post(&s);
	return NULL;
}


int main(void) {
	pthread_t tid;
	setbuf(stdout, NULL);

	sem_init(&s, 0, 0);

	printf("parent : before\n");

	pthread_create(&tid, NULL, child, NULL);
	sem_wait(&s);
	printf("parent : after\n");
	pthread_join(tid, NULL);

	printf("The end!\n");

	sem_destroy(&s);

	return EXIT_SUCCESS;
}