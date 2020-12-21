#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ctype.h>


int main(void) {
	int sizeString = 100;
	int fd1, fd2;
	char message[sizeString];
	int bytes;

	char* fifo1 = "fd1.fifo";
	char* fifo2 = "fd2.fifo";

	(void)umask(0);

	if (mknod(fifo1, S_IFIFO | 0666, 0) < 0) {
		fprintf(stderr, "mknod 1 error\n");
		return EXIT_FAILURE;
	}
	if (mknod(fifo2, S_IFIFO | 0666, 0) < 0) {
		fprintf(stderr, "mknod 2 error\n");
		return EXIT_FAILURE;
	}
	if ((fd1 = open(fifo1, O_WRONLY)) < 0) {
		fprintf(stderr, "open main error\n");
		return EXIT_FAILURE;
	}
	if ((fd2 = open(fifo2, O_RDONLY)) < 0) {
		fprintf(stderr, "open main2 error\n");
		return EXIT_FAILURE;
	}

	printf("Enter a string: ");
	fgets(message, sizeString, stdin);
	bytes = write(fd1, message, sizeString);
	if (bytes < 0) {
		fprintf(stderr, "parent write error\n");
		return EXIT_FAILURE;
	}
	bytes = read(fd2, message, sizeString);
	if (bytes < 0) {
		fprintf(stderr, "parent read error\n");
		return EXIT_FAILURE;
	}
	printf("UpperCase: %s", message);

	close(fd1);
	close(fd2);
	return EXIT_SUCCESS;
}