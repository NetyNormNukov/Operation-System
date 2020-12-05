#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

long val;
int flag = 0;
pthread_mutex_t mtx;
pthread_cond_t add;
pthread_cond_t sub;

void * increment(void * arg) {
	int num = *(int*) arg;
	int i;

	for (i = 0; i < num; i++) {
		pthread_mutex_lock(&mtx);
		while(flag == 1) {
			pthread_cond_wait(&add, &mtx);
		}
		val += 1;
		flag = 1;
		printf("\tIncrement thread: (%u), iteration: %d\n", (unsigned int)pthread_self(), i+1);
		//pthread_mutex_unlock(&mtx); //in 2018 change the order
	    //pthread_cond_signal(&sub);  //of this lines
	    pthread_cond_signal(&sub);  //This is much
	    pthread_mutex_unlock(&mtx); //safer strategy
	}

	return NULL;
}

void * decrement(void * arg) {
	int num = *(int*) arg;
	int i;

	for (i = 0; i < num; i++) {
		pthread_mutex_lock(&mtx);
		while(flag == 0) {
			pthread_cond_wait(&sub, &mtx);
		}
		val -= 1;
		flag = 0;
		printf("\t\tDecrement thread: (%u), iteration: %d\n", (unsigned int)pthread_self(), i+1);
		//pthread_mutex_unlock(&mtx); //in 2018 change the order
	    //pthread_cond_signal(&add);  //of this lines
	    pthread_cond_signal(&add);  //This is much
	    pthread_mutex_unlock(&mtx); //safer strategy
	}

	return NULL;
}

int main(void) {
	int num = 3;
	pthread_t tid1, tid2;
	setbuf(stdout, NULL);

	val = 0;
	pthread_mutex_init(&mtx, NULL);
	pthread_cond_init(&add, NULL);
	pthread_cond_init(&sub, NULL);

	printf("Before. val = %ld\n", val);
	

	pthread_create(&tid1, NULL, increment, &num);
	pthread_create(&tid2, NULL, decrement, &num);
	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);

	printf("After. val = %ld\n", val);

	pthread_mutex_destroy(&mtx);
	pthread_cond_destroy(&add);
	pthread_cond_destroy(&sub);

	return EXIT_SUCCESS;
}