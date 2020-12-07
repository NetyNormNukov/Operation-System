#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

char* messageTerm;

static void signal_handler(int signo)
{
	if (signo == SIGALRM)
	{	
		printf("Alarm signal was catch!\n")
		printf("%s\n", messageTerm);
		exit(0);
	}
}

int main(int argc, char const *argv[])
{	

	setbuf(stdout,NULL);
	if (signal(SIGALRM,signal_handler) == SIG_ERR)
	{
		fprintf(stderr, "%s\n", "Error SIGALRM init");
	}

	int delayClock = 3;

	if (argc == 3)
	{
		delayClock = atoi(argv[1]);
		messageTerm = argv[2];
	}
	else
	{
		fprintf(stderr, "Few arguments\n");
	}

	pid_t pid = fork();
	if (pid == 0)
	{
		alarm(delayClock);
		while(1)
		{
			pause();
		}
	}
	else if (pid > 0)
	{
		exit(0);
		printf("%s\n","bye Parent!)" );
	}
	else
	{
		fprintf(stderr, "Error fork\n" );
	}
	{
		while(1)
			printf("%s\n","hi");
	}



	return EXIT_SUCCESS;
}