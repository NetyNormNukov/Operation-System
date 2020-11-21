#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define MAIN_COUNT 0
#define THREAD_COUNT 0


void * any_func(void * args) {
	for (int i = 0; i < 10; ++i)
	{
		printf("	Child Thread. Iteration: %d\n", i);
		sleep(THREAD_COUNT);
	}
	sleep(5);
	return NULL;
}
int main(void) {
	setbuf(stdout, NULL);
	pthread_t thread;
	int result;
	
	if (pthread_create(&thread, NULL, &any_func, NULL)!= 0) {
		fprintf(stderr, "Error\n");
		return 1;
	}
	for (int i = 0; i < 10; ++i)
	{
		printf("Main Thread. Iteration: %d\n", i);
		sleep(MAIN_COUNT);
	}
	return EXIT_SUCCESS;
}
