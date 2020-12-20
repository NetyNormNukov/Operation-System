#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main(){

	int fd1[2], fd2[2], result;
	size_t size;
	int N = 100;
	char resstring[N], ch;
	
	if(pipe(fd1) < 0){
		printf("Can\'t create pipe\n");
		exit(-1);
	}
	if(pipe(fd2) < 0){
		printf("Can\'t create pipe\n");
		exit(-1);
	}
	//create new fork
	result = fork();
	while(1){
		if(result < 0){
			printf("Can't fork child\n");
			exit(-1);
		} else if (result > 0) {		
			//parent, peredaem dannie potomku
			//zakrivaem vihodnoi potok
			close(fd1[0]);
			close(fd2[1]);
			printf("Enter a string\n");
			//pitaemtsa zapisat v pipe
			fgets(resstring,N,stdin);

			if (strlen(resstring) == 0){
				exit(-1);
			}		
			//zapis v pipe
			size = write(fd1[1], resstring, N);
				if(size < N){
					printf("Can't write all string\n");
					exit(-1);
				}

			size = read(fd2[0], resstring, N);
			if(size < 0){
				printf("Can\'t read string\n");
				exit(-1);
			}
			printf("%s\n", resstring);
					
		} 
		else {		
		//child, zakrivaem vhodnoi potok
		close(fd1[1]);
		close(fd2[0]);

		//chitaem iz pipe		
		size = read(fd1[0], resstring, N);
		
		if(size < 0){
			printf("Can't read string\n");
			exit(-1);
		}

		int i = 0;
		while (resstring[i])
		{
			ch = resstring[i];
			resstring[i] = putchar(toupper(ch));
			i++;
		}
		size = write(fd2[1], resstring, N);
		
		if(size < 0){
			printf("Can't write string\n");
			exit(-1);
		}
	}
}
	
	close(fd1[0]);
	close(fd1[1]);
	close(fd2[0]);
	close(fd2[1]);
	return 0;
}
