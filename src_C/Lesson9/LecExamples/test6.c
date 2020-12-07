#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <wait.h>

#define NUM_TIMES 10000

/*
Tasks:
	Parent process waits while child starts and send test signal;
	Then parent send child the signal and child answers.

*/

volatile sig_atomic_t flag;

void exit_handler(void) {
	printf("Process %d has finished\n", (int)getpid());
}

void signal_handler(int signo) {
	printf("\t\tProcess %d. Catched signal %d (%s), N=%d\n", 
		(int)getpid(), signo, strsignal(signo), flag);
	if (signo == SIGTERM) {
		exit(EXIT_SUCCESS);
	} else if (signo == SIGUSR1) {
		flag += 1;
	}
}


int main(void) {
	setbuf(stdout, NULL);
	atexit(exit_handler);
	flag = 0;

	{
		pid_t pid;
		int res;
		sigset_t for_wait, for_work;
		static struct sigaction act;

		sigemptyset(&for_wait);
		sigaddset(&for_wait, SIGUSR1);
		sigaddset(&for_wait, SIGTERM);
		sigprocmask(SIG_SETMASK, &for_wait, NULL);

		sigfillset(&for_work);
		sigdelset(&for_work, SIGUSR1);
		sigdelset(&for_work, SIGTERM);

		sigfillset(&act.sa_mask);
		act.sa_handler = signal_handler;
		act.sa_flags = SA_RESTART;
		sigaction(SIGTERM, &act, NULL);
		sigaction(SIGUSR1, &act, NULL);

		printf("New Test Program\n");
		/**/
		pid = fork();

		if (pid > 0) {
			int status;
			printf("\tParent Process (%d). Child (%d)\n", (int)getpid(), pid);
			while(flag<NUM_TIMES) {
				kill(pid, SIGUSR1);
				sigsuspend(&for_work);
			}
			pid = wait(&status); 
			printf("\tChild with PID %d finishes ", (int)pid);
			if (WIFEXITED(status)) {
				printf("normally with code %d\n", WEXITSTATUS(status));
			} else if (WIFSIGNALED(status)) {
				printf("due to signal\n");
			}
			printf("\tParent Process stops\n");
		} else if (pid == 0) {
			printf("\t\tChild Process (%d). Parent (%d)\n", (int)getpid(), (int)getppid());
			while(flag<NUM_TIMES) {
				sigsuspend(&for_work);
				kill(getppid(), SIGUSR1);
			}
			printf("\t\tChild Process stops\n");
		} else {
			perror("After fork");
			exit(EXIT_FAILURE);
		}
	}

	return EXIT_SUCCESS;
}
