#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
	pid_t pid;

	setbuf(stdout, NULL);
	pid = fork();

	if (pid < 0) {
		fprintf(stderr, "ERROR!\n");
		perror("In main");
	} else if (pid == 0) {
		char *argv[] = {"one", "two", "three", NULL};
		char *env[] = {"FILE=Data.ini", NULL};
		execve("./child", argv, env);
		perror("In child process");
	} else {
		pid_t res;
		int status;
		printf("Parent process. PID(%d), Child PID(%d)\n", 
			getpid(), pid);
		res = waitpid(pid, &status, 0);
		if (res > 0) {
			printf("Parent wait for child with pid = %d\n", res);
			if (WIFEXITED(status)) {
				printf("Exit code: %d\n", WEXITSTATUS(status));
			}
		} else {
			perror("Wait in parent");
		}
	}

	return EXIT_SUCCESS;
}
