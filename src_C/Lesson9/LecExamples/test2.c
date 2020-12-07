#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <wait.h>
#include <signal.h>

volatile sig_atomic_t flag = 1;

static void signal_haldler(int signo) {
	flag = 0;
}

int main(void) {
	pid_t pid;
	setbuf(stdout,NULL);

	printf("Preparing actions in parent process PID(%d)\n", (int)getpid());
	if (signal(SIGUSR1, signal_haldler) == SIG_ERR) {
			fprintf(stderr, "\t\tIt is impossible to handle %s (%d)!\n", sys_siglist[SIGUSR1], SIGUSR1);
			perror("In main");
			exit(EXIT_FAILURE);
	}
	pid = fork();

	if (pid > 0) {
		int status;
		printf("In parent process PID(%d) after fork. Wait ater child PID(%d) make actions\n", (int)getpid(), pid);
		//sleep(5);
		while(flag) {
			pause();
		}
		printf("Parent - waiting is finished\n");

		pid = wait(&status);
		printf("Child PID(%d) finished\n", (int)pid);
		if (WIFEXITED(status)) {
			printf("Child finished with status: %d\n", WEXITSTATUS(status));
		} else if(WIFSIGNALED(status)) {
			printf("Child was terminated by a singal: %d\n", WTERMSIG(status));
		}
		printf("Parent finished\n");
	} else if (pid == 0) {
		//sleep(1);
		printf("\tСhild process PID(%d) for parent PID(%d) make actions after fork\n", (int)getpid(), (int)getppid());
		if(kill(getppid(), SIGUSR1)) {
			perror("In main child. kill");
		}
		printf("\tContinue after sending signal to parent\n");
	} else {
		perror("In main after fork");
		exit(EXIT_FAILURE);
	}

	return EXIT_SUCCESS;
}