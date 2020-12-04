#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


static long global = 0;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void * thread_increment(void * arg) {
	long num = *(long*)arg;
	long i, local;
	for (i = 0; i < num; i++) {
		if(pthread_mutex_lock(&mutex)){
			fprintf(stderr, "Mutex lock error\n");
			exit(EXIT_FAILURE);
		}
		global += 1;
		if(pthread_mutex_unlock(&mutex)){
			fprintf(stderr, "Mutex unlock error\n");
			exit(EXIT_FAILURE);
		}
	}
	return NULL;
}


void * thread_decrement(void * arg) {
	long num = *(long*)arg;
	long i, local;
	for (i = 0; i < num; i++) {
		pthread_mutex_lock(&mutex);
		global -= 1;
		pthread_mutex_unlock(&mutex);
	}
	return NULL;
}

int main(int argc, char ** argv) {
	long num = 1000;

	setbuf(stdout, NULL);
	if (argc == 2) {
		num = atol(argv[1]);
	}
	printf("Work with %ld iterations\n", num);
	printf("Initial gloval value: %ld\n", global);
	{
		int res;
		pthread_t tid1, tid2;

		if (res = pthread_create(&tid1, NULL, thread_increment, &num)) {
			fprintf(stderr, "Increment thread creation ERROR (%d)\n", res);
			exit(EXIT_FAILURE);
		}
		if (res = pthread_create(&tid2, NULL, thread_decrement, &num)) {
			fprintf(stderr, "Decrement thread creation ERROR (%d)\n", res);
			exit(EXIT_FAILURE);
		}

		pthread_join(tid1, NULL);
		pthread_join(tid2, NULL);
	}
	printf("Final gloval value: %ld\n", global);

	return EXIT_SUCCESS;
}