#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cv = PTHREAD_COND_INITIALIZER;
int done = 0;

void * child(void * arg) {
	printf("\tchild : work\n");
	pthread_mutex_lock(&mtx);
	done = 1;
	//pthread_cond_signal(&cv);
	pthread_mutex_unlock(&mtx);
	printf("\t\tSignal on cv\n");
	pthread_cond_signal(&cv);
	return NULL;
}


int main(void) {
	pthread_t tid;

	setbuf(stdout, NULL);
	printf("parent : before\n");

	pthread_create(&tid, NULL, child, NULL);
	
	pthread_mutex_lock(&mtx);
    while (done == 0) {
    	printf("\t\tSleep on CV\n");
    	pthread_cond_wait(&cv, &mtx);
    	printf("\t\tWake on CV\n");
    }
	pthread_mutex_unlock(&mtx);

	printf("parent : after\n");
	pthread_join(tid, NULL);

	printf("The end!\n");


	return EXIT_SUCCESS;
}