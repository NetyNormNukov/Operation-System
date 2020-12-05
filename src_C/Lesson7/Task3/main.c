#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <getopt.h>


typedef struct
{
	int* arr;
	pthread_rwlock_t lock;
	size_t countElem;
} argsThread;

void seed() {
	struct timespec now;
	if (clock_gettime(CLOCK_REALTIME, &now) < 0) {
		fprintf(stderr, "Error with time function!\n");
		exit(EXIT_FAILURE);
	}
	srand(now.tv_nsec);
}


void print_info()
{
	printf("%s\n", "-h or --help  get help page");
	printf("%s\n", "-c or --count set count of elements");
}
int getCountsOfDigits(int number) {
    int count = (number == 0) ? 1 : 0;
    while (number != 0) {
        count++;
        number /= 10;
    }
    return count;
}
void initThreadData( argsThread* data, size_t num)
{
	data->arr = (int*)malloc(num * sizeof(int));
	data->countElem = num;
	if (pthread_rwlock_init(&(data->lock), NULL))
	{
		printf("%s\n", "Error when init rwlock");
	}
}

void destroyThreadData(argsThread* data)
{
	free(data->arr);
	pthread_rwlock_destroy(&(data->lock));
}

void* detachedFunc(void* arg)
{

	argsThread* detArg = (argsThread*)arg;
	if (!detArg)
	{
		fprintf(stderr, "Error, can`t create struct\n");
	}
	while (1)
	{	
		//красивый вывод
		printf("\n____");
		for (int i = 0; i < detArg->countElem; ++i)
		{
			if (getCountsOfDigits(detArg->arr[i]) == 2)
			{
				printf("___");
			}
			if (getCountsOfDigits(detArg->arr[i]) == 1)
			{
				printf("__");
			}
		}
		printf("\nDet: ");
		for (int i = 0; i < detArg->countElem; ++i)
		{
			printf("%d ", detArg->arr[i]);
		}
		//красивый вывод
		printf("\n____");
		for (int i = 0; i < detArg->countElem; ++i)
		{
			if (getCountsOfDigits(detArg->arr[i]) == 2)
			{
				printf("___");
			}
			if (getCountsOfDigits(detArg->arr[i]) == 1)
			{
				printf("__");
			}
		}
		printf("\n");
		sleep(4);
	}

	return NULL;
}

void* writeFunc(void* arg) {
	argsThread* data = (argsThread*)arg;
	while (1) {
		pthread_rwlock_wrlock(&(data->lock));
		data->arr[rand() % data->countElem] = rand() % 99 + 1;
		pthread_rwlock_unlock(&(data->lock));
		sleep(4);
	}

	return NULL;
}

void* readFunc(void* arg) {
	argsThread* data = (argsThread*)arg;
	while (1)
	{	
		size_t index = rand() % data->countElem;
		pthread_rwlock_rdlock(&(data->lock));
		printf("Thread %lu read in index %lu value = %d\n", pthread_self(), index, data->arr[index]);
		pthread_rwlock_unlock(&(data->lock));
		sleep(2);
	}

	return NULL;
}

int main(int argc, char const *argv[])
{
	setbuf(stdout, NULL);
	size_t countElem = 100;

	if (argc != 1)
	{
		extern char* optarg;
		int c;
		struct option long_opt[] = {
			{"help",	0,	0,	'h'},
			{"count",	1,	0,	'c'},
			{NULL,		0,	NULL, 0 }
		};
		while ((c = getopt_long(argc, argv, "hc:", long_opt, NULL)) != -1)
		{
			switch (c)
			{
			case 'h':
				print_info();
				break;

			case 'c':
				countElem = atoi(optarg);
				printf("Count numbers = %ld\n", countElem);
				break;


			default:
				print_info();
			}
		}
	}


	//init writers and readers
	size_t countWriters = 5;
	size_t countReaders = 15;
	pthread_t* write = (pthread_t*)malloc((countWriters) * sizeof(pthread_t));
	pthread_t* read = (pthread_t*)malloc(countReaders * sizeof(pthread_t));
	//init data
	seed();
	argsThread dataStruct;
	initThreadData(&dataStruct, countElem);
	//init and create detached thread
	pthread_t pid;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	if (pthread_create(&pid, &attr, &detachedFunc, &dataStruct) != 0) {
		fprintf(stderr, "Error in pthread_create(detached)\n" );
	}
	//create thread
	for (int i = 0; i < countWriters; ++i)
	{
		if (pthread_create(&write[i], NULL, &writeFunc, &dataStruct) != 0) {
			fprintf(stderr, "Error pthread_create(writer)\n");
		}
	}
	for (int i = 0; i < countReaders; ++i)
	{
		if (pthread_create(&read[i], NULL, &readFunc, &dataStruct) != 0) {
			fprintf(stderr, "Error pthread_create(reader)\n");
		}
	}
	//wait thread
	void* result;
	for (int i = 0; i < countWriters; ++i)
	{
		if (pthread_join(write[i], &result) != 0) {
			fprintf(stderr, "Join error (writer)\n");
		}
	}
	for (int i = 0; i < countReaders; ++i)
	{
		if (pthread_join(read[i], &result) != 0) {
			fprintf(stderr, "Join error (reader)\n");
		}
	}
	// free all memory
	pthread_attr_destroy (&attr);
	free(write);
	free(read);
	destroyThreadData(&dataStruct);

	return EXIT_SUCCESS;
}