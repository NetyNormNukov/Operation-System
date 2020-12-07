/* Example of kill function */

#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#define WAIT 1

volatile sig_atomic_t ntimes = 0;


int main(void) {
	pid_t pid, ppid;
	void parent_action(int), child_action(int), alarm_action(int);
	
	/* Define handler of signal SIGALRM in both processes */
	if (signal(SIGALRM, alarm_action) == SIG_ERR) {
		fprintf(stderr, "It is impossible to handle SIGALRM!\n");
		exit(EXIT_FAILURE);
	}
	
	switch(pid = fork()) {
	case -1: /* Error */
		perror("task2");
		exit(EXIT_FAILURE);
	
	case 0: /* Child Process */
		/* Define handler of signal SIGUSR1 in child process */
		if (signal(SIGUSR1, child_action) == SIG_ERR) {
			fprintf(stderr, "It is impossible to handle SIGUSR1!\n");
			exit(EXIT_FAILURE);
		}
		/* get parent process id */
		ppid = getppid();
		
		/* Infinite loop */
		for(;;) {
			kill(ppid, SIGUSR1);
			alarm(WAIT);
			pause();
			alarm(0);
		}
		
	default: /* Parent Process */
		/* Define handler of signal SIGUSR1 in parent process */
		if (signal(SIGUSR1, parent_action) == SIG_ERR) {
			fprintf(stderr, "It is impossible to handle SIGUSR1!\n");
			exit(EXIT_FAILURE);
		}
		/* Infinite loop */
		for(;;) {
			alarm(WAIT);
			pause();
			alarm(0);
			kill(pid, SIGUSR1);
		}
	}
	return EXIT_SUCCESS;
}

/* ============================== */

void alarm_action(int sig) {
	printf("\tAlarm in PID(%d)\n", (int)getpid());
	raise(SIGUSR1);
}

void parent_action(int sig) {
	extern const char * const sys_siglist[];
	printf("Parent process (%d) get %s #%d\n", (int)getpid(), sys_siglist[sig], 
		++ntimes);
	alarm(0);
}

void child_action(int sig) {
	printf("Child process (%d) get %s #%d\n", (int)getpid(), sys_siglist[sig],
		++ntimes);
	alarm(0);
}