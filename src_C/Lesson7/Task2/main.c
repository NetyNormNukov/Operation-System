#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <getopt.h>
#include <math.h>

typedef struct
{
	double a;
	double b;
	double* e;
	pthread_mutex_t* mutex;
	double* I;
	int numSplit;

} argsThread;

static double func(double x)
{
	return (4 - x * x);
}

void help()
{
	printf("-n or --number  Set number of incr/decr\n");
	printf("-p or --pairs 	Set number of threads\n");
}

void* evalIntegral(void* arg)
{

	argsThread* args = (argsThread*)arg;
	double a = args->a;
	double b = args->b;
	double e = *(args->e);
	int numThread = args->numSplit;
	printf("a = %f,b = %f,e = %f\n",a,b,e);

	double In = 0.0;
	double I2n = In*2;

		while(1)
	{
		double h = (b-a)/numThread;
		for (double x = a; x <= b; x+=h)
		{
			I2n += func(x+h/2);
		}
		I2n*=h;

		if (fabs(In - I2n)<e)
		{
			break;
		}
		else
		{
			In = I2n;
			I2n = 0.0;
			numThread += 100;
		}
	}
	if(pthread_mutex_lock(args->mutex) != 0){
		fprintf(stderr, "Can`t lock mutex\n" );
		exit(EXIT_FAILURE);
	}
	(*(args->I))+=I2n;
	if(pthread_mutex_unlock(args->mutex) != 0){
		fprintf(stderr, "Can`t unlock mutex\n");
		exit(EXIT_FAILURE);
	}

	return NULL;
}


int main(int argc, char const *argv[])
{
	setbuf(stdout, NULL);
	double I = 0;
	double a = 0;
	double b = 2;
	double e = 1e-7;

	int numSplit = 10000;
	if (argc == 2)
	{
		numSplit = atoi(argv[1]);
	}

	double In = 0.0;
	double I2n = In * 2;
	double h = (b - a) / numSplit;
	
	pthread_t* threadsTid = (pthread_t*)malloc(numSplit * sizeof(pthread_t));
	argsThread* threadArgs = (argsThread*)malloc(numSplit * sizeof(argsThread));

	pthread_mutex_t mutex;

	if(pthread_mutex_init(&mutex, NULL) != 0){
		fprintf(stderr, "Error, can`t create mutex\n");
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < numSplit; ++i)
	{
		threadArgs[i].a = a + i * h;
		threadArgs[i].b = threadArgs[i].a + h;
		threadArgs[i].e = &e;
		threadArgs[i].mutex = &mutex;
		threadArgs[i].I = &I;
		threadArgs[i].numSplit = numSplit;
		if (pthread_create(&threadsTid[i], NULL, &evalIntegral, &threadArgs[i]) != 0){
			fprintf(stderr, "Error pthread_create()\n");
		}
		

	}

	void* result;

	for (int i = 0; i < numSplit; ++i)
	{
		if (pthread_join(threadsTid[i], &result) != 0) {
			fprintf(stderr, "Join error\n");
		}
	}

	printf("%f\n", I);
	// printf("%f\n", I);s
	pthread_mutex_destroy(&mutex);
	free(threadsTid);
	free(threadArgs);

	return EXIT_SUCCESS;
}