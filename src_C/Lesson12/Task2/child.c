#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
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

	if ((fd2 = open(fifo1, O_RDONLY)) < 0) {
		fprintf(stderr, "open 1 error\n");
		return EXIT_FAILURE;
	}
	if ((fd1 = open(fifo2, O_WRONLY)) < 0) {
		fprintf(stderr, "open 2 error\n");
		return EXIT_FAILURE;
	}

	
	bytes = read(fd2, message, sizeString);
	if (bytes < 0) {
		fprintf(stderr, "child write error\n");
		return EXIT_FAILURE;
	}
	int size = strlen(message);
	for (int i = 0; i < size; i++) {
		message[i] = toupper(message[i]);
	}
	bytes = write(fd1, message, sizeString);
	if (bytes < 0) {
		fprintf(stderr, "child read error\n");
		return EXIT_FAILURE;
	}
	
	close(fd1);
	close(fd2);

	return EXIT_SUCCESS;
}