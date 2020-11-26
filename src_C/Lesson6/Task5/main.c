#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>

static pthread_key_t key;

void* threadFunc(void* argument) {

	int countForLoop = rand() % 10 +1;
	char* getSpec = (char*)pthread_getspecific(key);
	if (pthread_getspecific(key) == NULL)
	{
		char* str = ", some text, random number:";
		pthread_t TID = pthread_self();

		char* specStr = (char*)malloc(sizeof str + sizeof TID + sizeof "Thread:%d"+100);//перевыделение памяти производиться специально.
		sprintf(specStr, "Thread:%d", pthread_self());	
		strcat(specStr,str);
		if (pthread_setspecific(key,specStr) != 0)
		{
			fprintf(stderr, "Error in Thread, pthread_setspecific()\n" );
		}

		getSpec = (char*)pthread_getspecific(key);	
		printf("___________________________________________________________________\n");
		// printf("specStr2 = %s\n",specStr);
		// free(specStr);//спросить.

	}
	
	
	for (int i = 0; i < countForLoop; ++i)
	{
		printf("This string is %d for %s%d\n",i+1, getSpec, rand() % 10 + 1);
	}

	return (void*) EXIT_SUCCESS;

}



int main(int argc, char* argv[]) {
	int countThreads = 1;

	if (argc == 2)
	{
		countThreads = atoi(argv[1]);
	}
	else {
		fprintf(stderr, "Error\n");
		return EXIT_FAILURE;
	}



	setbuf(stdout, NULL);
	pthread_key_create(&key,NULL);
	pthread_t thread[countThreads];

	for (int i = 0; i < countThreads; ++i)
	{
		if (pthread_create(&thread[i], NULL, &threadFunc, NULL) != 0) {
			fprintf(stderr, "Error pthread_create()\n");
			exit(EXIT_FAILURE);
		}
	}

	void* result = NULL;

	for (int i = 0; i < countThreads; ++i)
	{
		if (!pthread_equal(pthread_self(), thread[i]))
		{
			if (pthread_join(thread[i], &result) != 0) {
				fprintf(stderr, "Join error\n");
			}
		}
	}

	printf("Code EXIT = %d\n", result);
	if (result == PTHREAD_CANCELED)
		printf("Threads was canceled\n");
	else
		printf("Threads completed with code %d\n", result);

	return EXIT_SUCCESS;
}
