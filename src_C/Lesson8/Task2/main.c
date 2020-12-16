#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

typedef struct
{
	// pthread_cond_t cond;
	pthread_cond_t add;
	pthread_cond_t sub;
	pthread_mutex_t mutex;
	int data;
}dataThread;
int flag = 0;

void seed() {
	struct timespec now;
	if (clock_gettime(CLOCK_REALTIME, &now) < 0) {
		fprintf(stderr, "Error with time function!\n");
		exit(EXIT_FAILURE);
	}
	srand(now.tv_nsec);
}

void producerFunc(void* arg)
{
	dataThread* data = (dataThread*)arg;
	// printf("int data: %d\n",data->data);
	while(1)
	{
		int r = 1+rand()%100;
		pthread_mutex_lock(&(data->mutex));
		while(flag == 1) {
			pthread_cond_wait(&(data->add), &(data->mutex));
		}
		data->data = r;
		flag = 1;
		pthread_cond_signal(&(data->sub));
		pthread_mutex_unlock(&(data->mutex));
		sleep(1);
	}
}

void consumerFunc(void* arg)
{
	dataThread* data = (dataThread*)arg;
	// printf("int data2: %d\n",data->data);
	while(1)
	{
		int read;
		pthread_mutex_lock(&(data->mutex));
		while(flag == 0) {
			pthread_cond_wait(&(data->sub), &(data->mutex));
		}
		read = data->data;
		flag = 0;
		pthread_cond_signal(&(data->add));
		pthread_mutex_unlock(&(data->mutex));
		printf("%d\n", read);
		sleep(1);
	}
}

int main(int argc, char const *argv[])
{
	setbuf(stdout,NULL);
	int delay = 15;
	if (argc == 2)
	{
		delay = atoi(argv[1]);
		printf("Value of delay = %d\n", delay);
		printf("\n");
	}
	else
	{
		fprintf(stderr, "Cant receive value of delay\n" );
		printf("Value of delay = 15\n");
	}
	sleep(1);//for normal print string:Value of delay.
	//init data, cond, mutex;
	dataThread* data = (dataThread*)malloc(sizeof(dataThread));
	// if (pthread_cond_init(&(data->cond),NULL) != 0)
	// {
	// 	fprintf(stderr, "Error pthread_cond_init()\n");
	// 	return EXIT_FAILURE;
	// }
	if (pthread_cond_init(&(data->add),NULL) != 0)
	{
		fprintf(stderr, "Error pthread_cond_init()\n");
		return EXIT_FAILURE;
	}
	if (pthread_cond_init(&(data->sub),NULL) != 0)
	{
		fprintf(stderr, "Error pthread_cond_init()\n");
		return EXIT_FAILURE;
	}
	if(pthread_mutex_init(&(data->mutex),NULL) != 0)
	{
		fprintf(stderr, "Error pthread_mutex_init()\n" );
		return EXIT_FAILURE;
	}
	data->data = 0;

	pthread_t tidProd;
	pthread_t tidCons;
	seed();
	//create threads;
	if(pthread_create(&tidProd,NULL,&producerFunc,data) != 0)
	{
		fprintf(stderr, "Error pthread_create()\n");
		return EXIT_FAILURE;
	}
	if(pthread_create(&tidCons,NULL,&consumerFunc,data) != 0)
	{
		fprintf(stderr, "Error pthread_create()\n");
		return EXIT_FAILURE;
	}
	sleep(delay);
	//cancel threads
	if (pthread_cancel(tidProd) != 0)
	{
		fprintf(stderr, "Cant cancel thread\n" );
		sleep(3);
		pthread_cancel(tidProd);
		return EXIT_FAILURE;
	}
	if (pthread_cancel(tidCons) != 0)
	{
		fprintf(stderr, "Cant cancel thread\n" );
		sleep(3);
		pthread_cancel(tidCons);
		return EXIT_FAILURE;
	}
	
	//wait threads
	if(pthread_join(tidProd,NULL) != 0)
	{
		fprintf(stderr, "Join error\n");
	}
	if(pthread_join(tidCons,NULL) != 0)
	{
		fprintf(stderr, "Join error\n");
	}
	//free mem
	if(pthread_cond_destroy(&(data->add)) != 0)
	{
		fprintf(stderr, "Some thread still work with cond\n" );
	}
	if(pthread_cond_destroy(&(data->sub)) != 0)
	{
		fprintf(stderr, "Some thread still work with cond\n" );
	}
	if(pthread_mutex_destroy(&(data->mutex)) != 0)
	{
		fprintf(stderr, "Some thread still work with mutex\n" );
	}
	free(data);
	printf("The end)\n");
	return EXIT_SUCCESS;
}