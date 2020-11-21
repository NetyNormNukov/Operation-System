#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <errno.h>

int main (void) {
	int status;
	/* Child */
	pid_t pid = fork();
	if (pid == 0) {
		execlp("ls", "ls", NULL);
		fprintf (stderr, "Exec error\n");
	}
	else if(pid > 0)
	{
		/* Parent */
		printf("Before wait()\n");
		system("ps -l");
		sleep(10);
		wait(&status);
		sleep(1);
		printf("After wait()\n");
		system("ps -l");

		if (WIFEXITED (status)) {
			printf ("Code=%d\n", WEXITSTATUS(status));
		}
	}

	else
	{
		fprintf(stderr, "Fork error!\n");
		return -1;
	}
	return EXIT_SUCCESS;
}
