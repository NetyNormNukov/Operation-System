#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

// kill -s SIGUSR1 3422

volatile sig_atomic_t flag;

/* Handler for SIGINT */
static void signal_haldler(int signo) {
	flag = signo;
}

int main(void) {
	setbuf(stdout, NULL);
	printf("PID: %d\n", (int)getpid());
	printf("\tMaximun number of signals: %d\n", SIGRTMAX);
	printf("\tSignal handler registration\n");
	{
		int sig_num;
		for (sig_num = 1; sig_num <= SIGRTMAX; sig_num++) {
			if (signal(sig_num, signal_haldler) == SIG_ERR) {
				fprintf(stderr, "\t\tIt is impossible to handle %s (%d)!\n", sys_siglist[sig_num], sig_num);
				if (errno == EINVAL) {
					perror("In main");
					//exit(EXIT_FAILURE);
				}
			}
		}
	}
	printf("Wait for signals\n");
	while(1) {
		printf("The process is waiting for signal\n");
		alarm(5);
		pause();
		alarm(0);
		if (flag == SIGTERM) { 
			printf("Catched signal SIGTERM\n");
			break;
		}
		printf("\tThe signal %s (%d) was catched!\n", sys_siglist[flag], flag);
	}
		
	fprintf(stderr, "The program is finished\n");
	
	return EXIT_SUCCESS;
}