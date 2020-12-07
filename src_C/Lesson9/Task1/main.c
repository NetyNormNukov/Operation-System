#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

static void signal_handler(int signo)
{
	if (signo == SIGINT)
	{
		printf("%s\n", "Signal SIGINT");
	}
	else if (signo == SIGTERM)
	{
		printf("%s\n","Signal SIGTERM");
	}
	else if (signo == SIGUSR1)
	{
		exit(EXIT_SUCCESS);
	}
	else
	{
		fprintf(stderr, "Invalid signal\n" );
		exit(EXIT_FAILURE);
	}
}

int main(int argc, char const *argv[])
{	
	//use programm: compile and launch then launch new terminal. Command in terminal:
	//kill -l to view list signal with signo
	//kill -n [signo] [pid].
	//Use signal: 1) SIGHUP, 2) SIGINT, 10) SIGUSR1, 15) SIGTERM, 27) SIGPROF
	setbuf(stdout,NULL);
	printf("PID: %d\n",getpid());

	if (signal(SIGINT,signal_handler) == SIG_ERR)
	{
		fprintf(stderr, "Error SIGINT initialization!\n");
	}
	
	if (signal(SIGTERM,signal_handler) == SIG_ERR)
	{
		fprintf(stderr, "Error SIGTERM initialization!\n");
	}
	
	if (signal(SIGUSR1,signal_handler) == SIG_ERR)
	{
		fprintf(stderr, "Error SIGUSR1 initialization!\n");
	}
	
	if (signal(SIGPROF,SIG_DFL) == SIG_ERR)
	{
		fprintf(stderr, "Error SIGPROF initialization!\n");
	}
	
	if (signal(SIGHUP,SIG_IGN) == SIG_ERR)
	{
		fprintf(stderr, "Error SIGHUP initialization!\n");
	}
	
	while(1)
		pause();

	return EXIT_SUCCESS;
}