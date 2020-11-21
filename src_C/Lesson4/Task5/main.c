#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <unistd.h>
#include <errno.h>

extern char** environ;
extern int errno;

void print_info();



int main(int argc, char** argv)
{	
	size_t size_buffer = 127;

	if(argc != 1)
	{
		extern char* optarg;
		int c;
		struct option long_opt[] = {
				{"help",	0,	0,	'h'},
				{"buffer",	1,	0,	'b'},
				{NULL,		0,	NULL,0 }
		};
		while((c = getopt_long(argc, argv,"hb:", long_opt, NULL))!=-1)
		{
			switch(c)
			{
				case 'h':
					print_info();
				break;

				case 'b':
					size_buffer = atoi(optarg);
					printf(" buffer = %li\n", size_buffer);
				break;

				default:
					printf("%s\n","default");
			}
		}
	}
	char* buffer = (char*)malloc(size_buffer*sizeof(char));
	while(1)
	{
		printf("%s$>",getlogin());
		if(getline(&buffer, &size_buffer, stdin) != -1){
			int i = 0;//количество разделенных лексем
			int countSpace = 0;//количество пробелов
			int countOptions = 0;//количество опций
			if (strcmp(buffer,"stop\n") == 0)
			{
				printf("Wait 3 seconds...\n");
				free(buffer);
				break;
			}
			else
			{	
				pid_t pid = fork();
				if (pid == 0)
				{
					char* BufForBufForBuf [size_buffer-1];
					if (!BufForBufForBuf)
					{
						fprintf(stderr, "ERROR: Can`t allocate memory = %s\n", strerror(errno));
					}
					for (int j = 0; j < size_buffer-1; ++j)
					{
						BufForBufForBuf [j] = NULL;
					}
					
					char* BufForBuf = strtok(buffer, ";");//разделение по ;
					if (!BufForBuf)
					{
						fprintf(stderr, "ERROR: Can`t allocate memory = %s\n", strerror(errno));
					}
					while (BufForBuf != NULL) //пока есть лексемы
					{	
						BufForBufForBuf[i] = strdup(BufForBuf);//Массив разделенных по ;
					    BufForBuf = strtok(NULL, ";");
					    i++;

					}

					char* strWithDelim [size_buffer-1];//запись лишних элементов NULL
					if (!strWithDelim)
					{
						fprintf(stderr, "ERROR: Can`t allocate memory = %s\n", strerror(errno));
					}
					for (int j = 0; j < size_buffer-1; ++j)
					{
						strWithDelim [j] = NULL;
					}

					
					for (int j = 0; j < i; ++j)
					{
						char* BuffForStr = strtok(BufForBufForBuf[j], " ");//Массив разделенных по " "
						if (!BuffForStr)
						{
							fprintf(stderr, "ERROR: Can`t allocate memory = %s\n", strerror(errno));
						} 
						while (BuffForStr != NULL)
						{
							strWithDelim[countSpace] = strdup(BuffForStr);
							BuffForStr = strtok(NULL, " ");
							countSpace++;
						}
					}	

					for (int i = 0; i < size_buffer-1; ++i)
					{
						strtok(strWithDelim[i],"\n"); //удаление символа перевода строки
					}
					// char* optionsArray[countSpace];
					// char* argArray[countSpace];
					// int placeInArray = 0;
					// for (int i = 0; i < countSpace; ++i)
					// {
					// 	if (strWithDelim[i][0] == '-')
					// 	{
					// 		char* optionsArray[i] = strdup(strWithDelim[i]);
					// 		countOptions++; //подсчет количества опций
					// 	}
					// 	else
					// 	{
					// 		char* argArray[i] = strdup(strWithDelim[i]);
					// 		placeInArray = i;
					// 	}
					// }

					// printf("count = %d\n", countOptions);
					// if (countOptions != countSpace )//если опций больше чем количество слов-1
					// {
					// 	for (int i = 0; i < countSpace; ++i)
					// 	{
					// 		if (strWithDelim[i][0] != '-')
					// 		{	
					// 			execvp(strWithDelim[i], strWithDelim);
					// 			countSpace--;
					// 		}
					// 		else
					// 		{
					// 			execvp(strWithDelim[0], strWithDelim);
					// 		}
					// 	}
					// }else{
					// 	execvp(strWithDelim[0], strWithDelim);
					// 	fprintf(stderr, "execvp error\n" );
					// }
					execve(strWithDelim[0], strWithDelim, environ);
					execvp(strWithDelim[0], strWithDelim);
					fprintf(stderr, "ERROR exec() = %s\n", strerror(errno));

					//clear memory

					free(buffer);
					free(BufForBufForBuf);
					free(BufForBuf);
					free(strWithDelim);

					for (int j = 0; j < size_buffer-1; ++j)
					{
						free(BufForBufForBuf[j]);
					}
					free(BufForBufForBuf);
				}
				else if (pid > 0)
				{
					if (wait(NULL) == -1 && wait(NULL) == ECHILD)
					{
						fprintf(stderr, "ERROR Wait = %s\n", strerror(errno));
					}
				}
				else
					fprintf(stderr, "ERROR fork() = %s\n", strerror(errno));
			}
		}
		else
		{
			fprintf(stderr, "ERROR = %s\n", strerror(errno) );
			exit(EXIT_FAILURE);
		}
	}
	

	return EXIT_SUCCESS;
}

void print_info()
{
	printf("%s\n", "-h or --help get help page");
	printf("%s\n", "-b or --buffer set buffer size");
}
// if(BufForBuf[0] == '-')
// 				    {
// 				    	char* option = strdup(BufForBuf);
// 				    	// printf("option = %s\n",option);
// 				    	int countBeforWord;
// 				    	for(int i = 0; i < (strlen(BufForBuf)); i++)
// 				    	{
// 				    		// printf("sizeof = %ld\n", sizeof BufForBuf);
// 						    if (BufForBuf[i] == ';')
// 						    {	

// 						    	countBeforWord = i;
// 						    	// printf("in if\n" );
// 						    	// printf("BufForBuf[i] = %d\n",BufForBuf[i]);
// 						    	char* BufForBufForBuf;
// 						    	BufForBufForBuf = strdup(BufForBuf);

// 						    	char* delimArgs = strtok(BufForBufForBuf, ";");
// 						    	//qrona -aa;bbb -bbb; lom -aac
// 						    	// while(delimArgs != NULL)//del lom
// 						    	// {
// 						    	// 	char* delimArg = strdup(delimArgs);
// 						    	// 	// printf("delimArggg = %s\n", delimArg);
// 						    	// 	delimArgs = strtok(NULL, " ;");

// 						    	// }
// 						   	}

// 						}
// 				    }