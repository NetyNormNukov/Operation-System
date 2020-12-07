#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <wait.h>
#include <signal.h>

volatile sig_atomic_t flag1 = 0;
volatile sig_atomic_t flag2 = 1;

static void signal_haldler(int signo) {
	if (signo == SIGUSR1) {
		flag1 += 1;
	} else if (signo == SIGUSR2) {
		flag2 = 0;
	}
}


void handler_reg(int sig) {
	if (signal(sig, signal_haldler) == SIG_ERR) {
			fprintf(stderr, "\t\tIt is impossible to handle %s (%d)!\n", sys_siglist[sig], sig);
			perror("In handler_reg");
			exit(EXIT_FAILURE);
	}
}


int main(void) {
	pid_t pid;
	setbuf(stdout,NULL);

	printf("Preparing actions in parent process PID(%d)\n", (int)getpid());
	
	handler_reg(SIGUSR1);
	handler_reg(SIGUSR2);
	
	pid = fork();

	if (pid > 0) {
		int status;
		printf("In parent process PID(%d) after fork. Wait ater child PID(%d) make actions\n", (int)getpid(), pid);
		//sleep(5);
		while(flag2) {
			pause();
		}

		printf("Parent - waiting is finished.\n");

		pid = wait(&status);
		printf("Child PID(%d) finished\n", (int)pid);
		if (WIFEXITED(status)) {
			printf("Child finished with status: %d\n", WEXITSTATUS(status));
		} else if(WIFSIGNALED(status)) {
			printf("Child was terminated by a singal: %d\n", WTERMSIG(status));
		}
		printf("Parent finished. Number of SIGUSR1 received: %d\n", flag1);
	} else if (pid == 0) {
		int i;
		//sleep(1);
		printf("\tСhild process PID(%d) for parent PID(%d) make actions after fork\n", (int)getpid(), (int)getppid());
		for (i = 0; i < 1000; i++) { /*100*/
			if(kill(getppid(), SIGUSR1)) {
				perror("In main child. kill");
			}
		}
		if(kill(getppid(), SIGUSR2)) {
			perror("In main child. kill");
		}
		printf("\tContinue after sending signal to parent\n");
	} else {
		perror("In main after fork");
		exit(EXIT_FAILURE);
	}

	return EXIT_SUCCESS;
}