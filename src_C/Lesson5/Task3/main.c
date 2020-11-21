#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>

extern int errno;

struct thread_arg {
	char * str;
	int num;
};

struct thread_arg2 {
	int countNumbers;
	int desiredValue;
};

void * any_func(void * arg) {

	struct thread_arg targ =
	    *(struct thread_arg *) arg;
	for (int i = 0; i < targ.num; ++i)
	{
		printf("Child Thread.str = %s Iteration: %d\n",targ.str, i+1);
		
	}
	return NULL;
}
void* func(void* arg){

	struct thread_arg2 targ =
	    *(struct thread_arg2 *) arg;
	struct timespec t1;
	clock_gettime(CLOCK_REALTIME,&t1);
	srand(t1.tv_nsec);
	for (int i = 0; i < targ.countNumbers; ++i)
	{
		int randomValue = (int)rand() % 10 + 1;
		printf("Ramdom value = %d, iteration: %d\n", randomValue, i+1);
		if (randomValue == targ.desiredValue)
		{
			return NULL;
		}
	}
	return NULL;
}
int main(int argc, char* argv[]) {

	setbuf(stdout, NULL);
	pthread_t thread;
	pthread_t thread1;
	int countNumbers = atoi(argv[1]);

	struct thread_arg targ;
	
	targ.str = "str";
	targ.num = 10;

	struct thread_arg2 targ2;

	targ2.countNumbers = 10;
	targ2.desiredValue = 5;


	if (pthread_create(&thread, NULL, &any_func, &targ)!= 0) {
		fprintf(stderr, "Error pthread_create\n");
		exit(EXIT_FAILURE);
	}
	if (pthread_create(&thread1, NULL, &func, &targ2)!= 0){
		fprintf(stderr, "Error pthread_create\n");
		exit(EXIT_FAILURE);
	}
	if (pthread_join(thread, NULL) != 0) {
		fprintf(stderr, "Join error\n");
		exit(EXIT_FAILURE);
	}
	if (pthread_join(thread1, NULL) != 0) {
		fprintf(stderr, "Join error\n");
		exit(EXIT_FAILURE);
	}
	printf("EXIT_SUCCESS in programm\n");
	sleep(1);
	return EXIT_SUCCESS;
}

