#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <getopt.h>

long glob = 0;

void help()
{
	printf("-n or --number  Set number of incr/decr\n");
	printf("-p or --pairs 	Set number of threads\n");
}

void* threadIncr(void* arg)
{
	long num = *(long*)arg;
	for (int i = 0; i < num; ++i)
	{
		glob++;
	}
}

void* threadDecr(void* arg)
{
	long num = *(long*)arg;
	for (int i = 0; i < num; ++i)
	{
		glob--;
	}
}

int main(int argc, char const *argv[])
{
	if (argc < 2)
	{
		help();
		return 0;
	}
	extern char* optarg;
	char* optstring = "hn:p:";

	int c;
	long num = 10000;
	int numPairs = 2;
	struct option long_opt[] = {
		{"help", 0, 0, 'h'},
		{"number", 1, 0, 'n'},
		{"pairs", 1, 0, 'p'},
		{NULL, 0, NULL, 0}
	};
	while ((c = getopt_long(argc, argv, optstring, long_opt, NULL)) != -1)
	{
		switch (c)
		{
		case 'h':
			help();
			return 0;

		case 'n':
			num = atoi(optarg);
			break;

		case 'p':
			numPairs = atoi(optarg);
			break;

		default:
			help();
			return 0;
		}
	}

	pthread_t* threadTid = (pthread_t*)malloc(numPairs * 2 * sizeof(pthread_t));
	printf("Before threads:%ld\n", glob);
	for (int i = 0; i <= numPairs; i += 2)
	{
		
		if (pthread_create(&threadTid[i], NULL, &threadIncr, &num) != 0) {
			fprintf(stderr, "Error pthread_create()\n");
			exit(EXIT_FAILURE);
	}
		if (pthread_create(&threadTid[i + 1], NULL, &threadIncr, &num) != 0) {
			fprintf(stderr, "Error pthread_create()\n");
			exit(EXIT_FAILURE);
	}

	for (int i = 0; i <= numPairs; i += 2)
	{
		if (pthread_join(threadTid[i], NULL) != 0) {
			fprintf(stderr, "Join error\n");
			exit(EXIT_FAILURE);
		}
		if (pthread_join(threadTid[i + 1], NULL) != 0) {
			fprintf(stderr, "Join error\n");
			exit(EXIT_FAILURE);
		}
	}
	printf("After threads:%ld\n", glob);
	free(threadTid);

	return 0;
}