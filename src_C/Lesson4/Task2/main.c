#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char const *argv[]) 
{
	int delay = 5;
	long iter = 0;

	if (argc == 2)
	{
		delay = atoi(argv[1]);
	}
	printf("%d\n", delay);
	pid_t pid = fork();
	time_t now = time(NULL);
	while(time(NULL) - now <= delay)
	{
		iter+=1;
	}
	if (pid == 0)
		printf("In child iter = %ld\n", iter);
	else if (pid > 0)
		printf("In parent iter = %ld\n", iter);	
	else
		printf("Ошибка вызова fork, потомок не создан\n");
	return EXIT_SUCCESS;
}