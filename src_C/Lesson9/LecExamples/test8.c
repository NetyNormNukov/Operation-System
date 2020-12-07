#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

#define SIGNAL_COUNT 10

volatile sig_atomic_t final_flag = 1;
volatile sig_atomic_t synchro_flag = 0;

void synchro_handler(int signo) {
	synchro_flag = 1;
    printf("PID: %d, obtain synchro signal: %s.\n", (int)getpid(), sys_siglist[signo]);
}


void child_handler(int signo, siginfo_t *info, void *extra) {
	if(signo == SIGTERM) {
		fprintf(stderr, "\t\tProcess %d get TERM signal\n", (int)getpid());
		exit(EXIT_SUCCESS);
	} else {
		printf("\tChild PID: %d, signal: %s, from PID: %d, value: [%d]\n", 
		              (int)getpid(), strsignal(signo), (int)info->si_pid, info->si_value.sival_int);
		if (info->si_value.sival_int == (SIGNAL_COUNT-1)) final_flag = 0;
	}
}

int main(void) {	
	static sigset_t new_mask, zero_mask;
	
	setbuf(stdout, NULL);	
	
	//Synchro settings for parent. Make here to avoid race conditions with child
	if (signal(SIGUSR1, synchro_handler) == SIG_ERR) {
		fprintf(stderr, "Error invoking signal(SIGUSR1)\n");
		exit(EXIT_FAILURE);
	}
	
	sigemptyset(&zero_mask);
	sigemptyset(&new_mask);
	sigaddset(&new_mask, SIGUSR1);
	if (sigprocmask(SIG_BLOCK, &new_mask, NULL) < 0) {
		fprintf(stderr, "Error invoking sigprocmask(SIG_BLOCK)\n");
		exit(EXIT_FAILURE);
	}
	
	//Make child process
	pid_t child_pid = fork();
	if(child_pid == 0) {
		// Settings for signal exchange
		sigset_t mask_block, mask_get;
	    struct sigaction action;
		
		sigemptyset(&mask_block);
		sigaddset(&mask_block, SIGRTMIN);
		
		sigfillset(&mask_get);
		sigdelset(&mask_get, SIGRTMIN);
		sigdelset(&mask_get, SIGINT);
		sigdelset(&mask_get, SIGTERM); 
	
		sigfillset(&action.sa_mask); 
		action.sa_flags = SA_SIGINFO;
		action.sa_sigaction = child_handler;
		
		sigaction(SIGRTMIN, &action, NULL);
		sigaction(SIGTERM, &action, NULL);
		
		sigprocmask(SIG_BLOCK, &mask_block, NULL);
		
		printf("\tChild ready to work\n");
		//Tell parent that child is ready
		kill(getppid(), SIGUSR1);
		
		// Wait signals from parent
		while(final_flag) {
			sigsuspend(&mask_get);
		}
		
		//Tell parent that child has finished its work
		kill(getppid(), SIGUSR1);
		//Wait for parent answer - stopping the application
		// For this program organizing it is not neccessary
		sigsuspend(&mask_get);
	}
	else
	{
		
		int i, status;
		union sigval v;

		printf("Parent PID: %d, Child PID: %d\n", getpid(), child_pid);
		//wait child to finish its settings
		while(synchro_flag == 0)
			sigsuspend(&zero_mask);
		synchro_flag = 0;
		printf("Parent ready to work\n");		
		for(i = 0; i < SIGNAL_COUNT; i++) {
			v.sival_int = i;
			int res = sigqueue(child_pid, SIGRTMIN, v);
			printf("Parent send signal number %d to PID %d\n", i, child_pid);
		}
		
		//wait child to finish its work
		while(synchro_flag == 0)
			sigsuspend(&zero_mask);
		synchro_flag = 0;
		
		sigqueue(child_pid, SIGTERM, (const union sigval) NULL); // It is not neccessary
		wait(&status);
		printf("Thats All!!!\n");
	}
	
	return EXIT_SUCCESS;
}