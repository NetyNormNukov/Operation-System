#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

extern const char * const sys_siglist[];

pid_t pid;
static void signal_handler(int signo)
{
	if (signo == SIGUSR1)
	{	
		printf("Захвачен сигнал %s\n", sys_siglist[signo]);
		sleep(1);
		if(pid==0){
            printf("Child process message!\n");
            kill(getppid(),SIGUSR1);
        }else{
            printf("Parent process message!\n");
            kill(pid,SIGUSR1); 
        }
	}
}

int main(int argc, char const *argv[])
{	

	if (signal(SIGUSR1,signal_handler) == SIG_ERR)
	{
		fprintf(stderr, "%s\n", "Error SIGUSR1 initialization");
	}

	pid = fork();
	if (pid == 0)
	{	
		printf("Child process %d\n",getpid());
		while(1)
		{
			pause();
		}
	}
	else if (pid > 0)
	{	
		printf("Parent process %d\n",getpid());
		kill(pid,SIGUSR1);
		while(1)
		{
			pause();
		}
	}
	else
	{
		fprintf(stderr, "Error fork\n" );
	}
	return EXIT_SUCCESS;
}