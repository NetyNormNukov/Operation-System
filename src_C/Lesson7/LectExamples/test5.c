#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

typedef struct {
	double temper;
	pthread_rwlock_t lock;
} Thermometer;

void init_thermometer(Thermometer * t) {
	t->temper = 0;
	if(pthread_rwlock_init(&(t->lock), NULL)) {
		fprintf(stderr, "Cannot initialize rwlock!\t");
		exit(EXIT_FAILURE);
	}
}

void destroy_thermometer(Thermometer * t) {
	t->temper = 0;
	pthread_rwlock_destroy(&(t->lock));
}

void set_temperature(Thermometer * t, double val) {
	pthread_rwlock_wrlock(&(t->lock));
	t->temper = val;
	pthread_rwlock_unlock(&(t->lock));
}

double get_temperature(Thermometer * t) {
	double res;
	pthread_rwlock_rdlock(&(t->lock));
	res = t->temper;
	pthread_rwlock_unlock(&(t->lock));
	return res;
}


void * write_thread(void * arg) {
	Thermometer * t = (Thermometer *)arg;
	int k;
	double val;
	printf("\tWriter Thread %u is started\n", (unsigned int)pthread_self());
	for (k = 0; k < 10; k++) {
		val = -2.0 + (double)rand() / RAND_MAX * 20.0;
		set_temperature(t, val);
		printf("\tWriter Thread %u, val = %g\n", (unsigned int)pthread_self(), val);
		sleep(3);
	}
	printf("\tWriter Thread %u is stopped\n", (unsigned int)pthread_self());
	return NULL;
}

void * read_thread(void * arg) {
	Thermometer * t = (Thermometer *)arg;
	int k;
	double res;
	printf("\t\tReader Thread %u is started\n", (unsigned int)pthread_self());
	for (k = 0; k < 25; k++) {
		res = get_temperature(t);
		printf("\t\tReader Thread %u, val = %g\n", (unsigned int)pthread_self(), res);
		sleep(1);
	}
	printf("\t\tReader Thread %u is stopped\n", (unsigned int)pthread_self());
	return NULL;
}


int main(void) {
	pthread_t w, r[5];
	Thermometer t;
	int i;

	setbuf(stdout, NULL);
	srand(time(NULL));

	printf("Program started\n");
	init_thermometer(&t);

	if (pthread_create(&w, NULL, &write_thread, &t)) {
		fprintf(stderr, "Writer Thread Creation Error\n");
		exit(EXIT_FAILURE);
	}

	for (i = 0; i < 5; i++) {
		if (pthread_create(&r[i], NULL, &read_thread, &t)) {
			fprintf(stderr, "Reader Thread %d Creation Error\n", i);
			exit(EXIT_FAILURE);
		}
	}

	if (pthread_join(w, NULL)) {
		fprintf(stderr, "Writer Thread Waiting Error\n");
		exit(EXIT_FAILURE);
	}
	for (i = 0; i < 5; i++) {
		if (pthread_join(r[i], NULL) != 0) {
			fprintf(stderr, "Reader Thread %d Waiting Error\n", i);
			exit(EXIT_FAILURE);
		}
	}

	printf("Program finished\n");
	destroy_thermometer(&t);

	return EXIT_SUCCESS;
}