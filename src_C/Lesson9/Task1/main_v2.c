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
		fprintf(stderr, "signal:SIGUSR1, EXIT_SUCCESS!!!\n" );
		exit(EXIT_SUCCESS);
	}
	else
	{
		fprintf(stderr, "Invalid signal\n" );
		exit(EXIT_FAILURE);
	}
}

int main(int argc, char const *argv[])
{	//use programm: compile and launch then launch new terminal. Command in terminal:
	//kill -l to view list signal wit signo
	//kill -n [signo] [pid].
	//Use signal: 1) SIGHUP, 2) SIGINT, 10) SIGUSR1, 15) SIGTERM, 27) SIGPROF

	setbuf(stdout,NULL);
	printf("PID = %d\n",getpid());

	struct sigaction action = {signal_handler,NULL,NULL,NULL,NULL};
	struct sigaction actionDFL = {SIG_DFL,NULL,NULL,NULL,NULL};
	struct sigaction actionIGN = {SIG_IGN,NULL,NULL,NULL,NULL};

	if (sigaction(SIGINT,&action,NULL) != 0)
	{
		fprintf(stderr, "%s\n", "Error SIGINT initialization!");
	}
	if (sigaction(SIGTERM,&action,NULL) != 0)
	{
		fprintf(stderr, "%s\n", "Error SIGTERM initialization!");
	}
	if (sigaction(SIGUSR1,&action,NULL) != 0)
	{
		fprintf(stderr, "%s\n", "Error SIGUSR1 initialization!");
	}
	if (sigaction(SIGPROF,&actionDFL,NULL) != 0)
	{
		fprintf(stderr, "%s\n", "Error SIGPROF initialization!");
	}
	if (sigaction(SIGHUP,&actionIGN,NULL) != 0)
	{
		fprintf(stderr, "%s\n", "Error SIGHUP initialization!");
	}

	while(1)
		pause();
	return EXIT_SUCCESS;
}