/*
trylock example
*/

//https://manpages.debian.org/stretch/glibc-doc/pthread_mutex_trylock.3.en.html
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>

#define MAX_VAL 3

static int arr[MAX_VAL] = {0};
static pthread_mutex_t mtx[MAX_VAL] = {PTHREAD_MUTEX_INITIALIZER};


void * thread(void * arg) {
	int num, i, ind1, ind2, val, res;
	num = 15000;
	i = 0;
	while (i < num) {
		/*printf("TID %u, val = %d\n", (unsigned int)pthread_self(), rand() % num);*/
		ind1 = rand() % MAX_VAL; val = rand() % (10*num);
		//pthread_mutex_lock(mtx+ind1);
		if(res = pthread_mutex_trylock(mtx+ind1)) {
			if (res == EBUSY) {
				printf("\t\tTID %u, mutex index = %d is busy, val = %d\n", (unsigned int)pthread_self(), ind1, val);
				continue;
			}
		}
		arr[ind1] = val;
		printf("\tTID %u, index = %d, val = %d\n", (unsigned int)pthread_self(), ind1, val);
		ind2 = rand() % MAX_VAL; val = rand() % (10*num);
		//pthread_mutex_lock(mtx+ind2);
		if(res = pthread_mutex_trylock(mtx+ind2)) {
			if (res == EBUSY) {
				printf("\t\tTID %u, mutex index = %d is busy, val = %d\n", (unsigned int)pthread_self(), ind2, val);
				pthread_mutex_unlock(mtx+ind1);
				printf("\t\tTID %u, free mutex with index = %d\n", (unsigned int)pthread_self(), ind1);
				continue;
			}
		}
		arr[ind2] = val;
		printf("\tTID %u, index = %d, val = %d\n", (unsigned int)pthread_self(), ind2, val);
		pthread_mutex_unlock(mtx+ind1);
		pthread_mutex_unlock(mtx+ind2);
		i += 1;
		//sleep(2);
	}
	return NULL;
}


int main(void) {
	setbuf(stdout, NULL);
	srand(time(NULL));
	printf("Process started\n");
	{
		pthread_t tid1, tid2;
		if (pthread_create(&tid1, NULL, thread, NULL)) {
			fprintf(stderr, "Thread 1 creation error");
			exit(EXIT_FAILURE);
		}
		if (pthread_create(&tid2, NULL, thread, NULL)) {
			fprintf(stderr, "Thread 2 creation error");
			exit(EXIT_FAILURE);
		}

		pthread_join(tid1, NULL);
		pthread_join(tid2, NULL);
	}
	printf("Process finished\n");
	return EXIT_SUCCESS;
}