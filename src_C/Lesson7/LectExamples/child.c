#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[]) {
	int i;
	setbuf(stdout, NULL);
	
	for (i = 0; i < argc; i++)
		printf("\t%s\n", argv[i]);

	printf("FILE = %s\n", getenv("FILE"));

	return EXIT_SUCCESS;
}