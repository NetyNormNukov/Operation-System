
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char const *argv[])
{
	if (argc == 1)
	{
		printf("\nPID = %d\n",getpid());
		printf("PPID = %d\n",getppid());
		printf("group ID = %d\n",getpgrp());
		printf("UID = %d\n", getuid());
		printf("GID = %d\n", getgid());
		printf("eUID = %d\n", geteuid());
		printf("eGID = %d\n", getegid());
	}

	if (argc == 2)
	{
		int numArg = atoi(argv[1]);

		switch(numArg){
			case 1:
				printf("\nPID = %d\n",getpid());
			break;
			case 2:
				printf("PPID = %d\n",getppid());
			break;
			case 3:
				printf("group ID = %d\n",getpgrp());
			break;
			case 4:
				printf("UID = %d\n", getuid());
			break;
			case 5:
				printf("GID = %d\n", getgid());
			break;
			case 6:
				printf("eUID = %d\n", geteuid());
			break;
			case 7:
				printf("eGID = %d\n", getegid());
			break;
			default:
				printf("Enter a digit in range 1-7\n" );

		}
		// printf("%d\n", numArg);
	}

	return 0;
}