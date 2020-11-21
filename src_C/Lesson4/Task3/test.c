#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(void)
{	
	int arr[3];
	pid_t pid = fork();
	sleep(1);
	// pid = fork();
	if (pid == 0)
	{
		sleep(1);
		arr[0] = getpid();
        printf("Child in child my pid = %i, parent pid = %i\n", getpid(), getppid());
    	
    }
    else if(pid<0)
        perror("fork");
    else
    {
        printf("Parent in child my pid = %i, parent pid = %i\n", getpid(), getppid());
        arr[1] = getpid();
    }
    // system("ps");

    char* str = (char*) malloc (sizeof("kill ") + sizeof(int));
    str = NULL;
    for(int i = 0; i < 1; i++)
    {	
    	sleep(2);
    	// printf("%i\n", i);
    	char numberInt[10] = {};
    	printf("numberInt = %s\n", numberInt);
    	str = (char*) realloc (str, sizeof("kill " + sizeof numberInt));
    	strcat(str, "kill ");
    	printf("str = %s\n",str);
    	printf("arr[0] = %d\n", arr[0]);
    	sprintf(numberInt, "%d", arr[i]);
    	printf("numberInt = %s\n", numberInt);
    	strcat(str, numberInt);
    	printf("STRING = %s\n",str );
    	system("ps");
    	system(str);
    	str = NULL;
    }
    


}