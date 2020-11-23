#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <wait.h>
#include <math.h>


int main(int argc, char* argv[]){ 
	//задание начальных значений
	int childCount = 1;

	char command[14] = "./childpr";

	char* radString = (char*) malloc(sizeof(char) * 4);//строка с радиусом
	if(!radString){
		fprintf(stderr, "Error, can`t allocate memory\n");
		return EXIT_FAILURE;
	}
	strcpy(radString, "5.0");

	char* numStr = (char*) malloc(sizeof(char) * 3);//строка с числом точек
	if(!numStr){
		fprintf(stderr, "Error, can`t allocate memory\n");
		return EXIT_FAILURE;
	}	
	strcpy(numStr, "10");

	int opt;
	char* buffer;
	while((opt = getopt(argc, argv, "r:n:c:")) != -1){
		switch(opt){
			case 'r':
				buffer = realloc(radString, sizeof(char) * (strlen(optarg) + 1));
				if(!buffer){
					fprintf(stderr, "Error, can`t reallocate memory\n");
					return EXIT_FAILURE;
				}
				radString = buffer;
				strcpy(radString, optarg);
				break;
			case 'n':
				buffer = realloc(numStr, sizeof(char) * (strlen(optarg) + 1));
				if(!buffer){
					fprintf(stderr, "Error, can`t reallocate memory\n");
					return EXIT_FAILURE;
				}
				numStr = buffer;
				strcpy(numStr, optarg);
				break;
			case 'c':
				childCount = atoi(optarg);
				break;
		}
	}	

	double rad = atof(radString);			//радиус
	double n = atof(numStr);				//количество точек
	double Sp = (2 * rad) * (2 * rad);		//площать квадрата, описанного вокруг круга
	double results[childCount];				//массив результатов

	//создание потомков
	for(int i = 0; i < childCount; i++){
		pid_t pid = fork();
		if(!pid){		
			execl(command, command, "-r", radString, "-n", numStr, NULL);
			fprintf(stderr, "Execl() error\n");
			exit(EXIT_FAILURE);
		}
		else if(pid > 0){
			int status = 0;
			wait(&status);
			int m;
			if(WIFEXITED(status))
				m = WEXITSTATUS(status);			//количество точек, попавших в область круга
			else{
				fprintf(stderr, "Uncorrect process exit\n");
				exit(EXIT_FAILURE);
			}			
			results[i] = (m/n)*Sp;				//приблизительная площадь круга
		}
		else{
			fprintf(stderr, "Fork() error\n");
			exit(EXIT_FAILURE);
		}
	}
	free(radString);
	free(numStr);
	
	//среднее значение
	double S = 0.0;
	for(int i = 0; i < childCount; i++)
		S += results[i];	
	S /= childCount;
	
	double pi = S/(rad * rad);
	double error = fabs(M_PI-pi);
	printf("Circle area: %.4f\n", S); 
	printf("PI: %.4f\n", pi);
	printf("Error in calculating pi: %.4f\n", error);
	return EXIT_SUCCESS;
}

