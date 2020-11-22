#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <getopt.h>
#include <math.h>
#include <errno.h>

extern int errno;

typedef struct {
	size_t count;
	unsigned long long* arr;
}threadArgs;

static unsigned long long factorial (int n)
{
	return (n < 2) ? 1 : n * factorial (n - 1);
}

static int is_simple(long n){
	for(long i = 2; i <= sqrt(n); i++)
		if(!(n % i))
			return 0;	
	return 1;
}

void print_info()
{
	printf("%s\n", "-h or --help get help page");
	printf("%s\n", "-s or --simple set count of simple numbers");
	printf("%s\n", "-k or --katalan set count of katalana numbers");
}

void* katalankNumbersFunc(void * arg) {

	threadArgs* args = (threadArgs*) arg;
	if (!args)
	{
		fprintf(stderr, "Error in initialization thread arguments\n");
	}
	for (int i = 0; i <= args->count; ++i)
	{
		args->arr[i] = factorial(2 * i) / (factorial(i + 1) * factorial(i));
	}

	return (void*) args->arr;
}

void* simpleNumbersFunc(void* arg) {

	threadArgs* args = (threadArgs*) arg;
	if (!args)
	{
		fprintf(stderr, "Error in initialization thread arguments \n" );
	}

	int counter = 0;
	for(long n = 0; counter < args->count; n++){
		if(is_simple(n)){
			args->arr[counter] = n;
			counter++;
		}

	}

	return (void*) args->arr;
}
int main(int argc, char* argv[]) {

	setbuf(stdout, NULL);
	pthread_t simpleThread;
	pthread_t katalanThread;

	size_t countSimpleNumber = 10;
	size_t countKatalanaNumber = 10;


	if (argc != 1)
	{
		extern char* optarg;
		int c;
		struct option long_opt[] = {
			{"help",	0,	0,	'h'},
			{"simple",	1,	0,	's'},
			{"katalan",	1,	0,	'k'},
			{NULL,		0,	NULL, 0 }
		};
		while ((c = getopt_long(argc, argv, "hs:k:", long_opt, NULL)) != -1)
		{
			switch (c)
			{
			case 'h':
				print_info();
				break;

			case 's':
				countSimpleNumber = atoi(optarg);
				printf("Count of simple numbers = %ld\n", countSimpleNumber);
				break;

			case 'k':
				countKatalanaNumber = atoi(optarg);
				printf("Count of katalana numbers = %ld\n", countKatalanaNumber);
				break;

			default:
				print_info();
			}
		}
	}

	unsigned long long* katalanaArr =
	 (unsigned long long*) malloc((countKatalanaNumber)*sizeof(unsigned long long));

	unsigned long long* simpleArr =
	 (unsigned long long*)malloc(countSimpleNumber*sizeof(unsigned long long));

	threadArgs katalanArgs = {countKatalanaNumber, katalanaArr};
	threadArgs simpleArgs = {countSimpleNumber, simpleArr};

	if (pthread_create(&simpleThread, NULL, &simpleNumbersFunc, &simpleArgs) != 0) {
		fprintf(stderr, "Error pthread_create\n");
		exit(EXIT_FAILURE);
	}
	if (pthread_create(&katalanThread, NULL, &katalankNumbersFunc, &katalanArgs) != 0) {
		fprintf(stderr, "Error pthread_create\n");
		exit(EXIT_FAILURE);
	}

	void* buffResult = NULL;
	

    //wait threads
	if (pthread_join(simpleThread, &buffResult) != 0) {
		fprintf(stderr, "Join error\n");
		exit(EXIT_FAILURE);
	}

	if(buffResult)
		simpleArr = (unsigned long long*)buffResult;

	buffResult = NULL;
	if (pthread_join(katalanThread, &buffResult) != 0) {
		fprintf(stderr, "Join error\n");
		exit(EXIT_FAILURE);
	}
	
	if(buffResult)
		katalanaArr = (unsigned long long*)buffResult;



	//printing results
	printf("First %ld Katalan digits: \n", countKatalanaNumber);
	for (int i = 0; i < countKatalanaNumber; ++i)
	{
		printf("%llu ",katalanaArr[i]);
	}
	printf("\n");
	printf("First %ld simple digits: \n", countSimpleNumber);
	for (int i = 0; i < countSimpleNumber; ++i)
	{
		printf("%llu ", simpleArr[i]);
	}
	printf("\n");
	//free memory
	free(simpleArr);
	free(katalanaArr);
	printf("EXIT_SUCCESS in programm\n");
	sleep(1);

	return EXIT_SUCCESS;
}

