#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include "func.h"

extern char** environ;

int main(int argc, char **argv) {
  char* assign = NULL;
  char* value  = NULL;

	if(argc == 1) { // если запускаем без аргументов, выводим environ
    int i = 0;
		while (environ[i] != NULL) {
        	printf("%s\n", environ[i]);
        	i++;
        };
		return 0;
	}

  else{  
    static struct option long_options[] =
         { 
            {"help",    no_argument,       0, 'h'},
            {"clear",   no_argument,       0, 'c'},
            {"info",    required_argument, 0, 'i'},
            {"set",     required_argument, 0, 's'},
            {"assign",  required_argument, 0, 'a'},
            {"value",   required_argument, 0, 'v'},
            {"del",     required_argument, 0, 'd'},
            {0, 0, 0, 0}
          };
    int option_index = 0;
    int c;
  	while ((c = getopt_long(argc,argv,"hc:i:s:a:v:d",long_options,NULL))!=-1){
        
        switch (c)
          {
          case 'i':
            show_info_a_variable(optarg);
            break;

          case 'h':
            infoUsable();
            break;

          case 's':
            setValue(optarg);
            break;

          case 'a':
            assign = optarg;
            //должна быть хитрая-хитрая проверка 🙃
            break;

          case 'v':
            value = optarg;
            break;

          case 'd':
            deleteEnv(optarg);
            break;

          case 'c':
            cleanEnv();
            break;

          default:
            infoUsable();
          }
          
      }
      printf("assign = %s, value = %s\n",assign,value );
      putInEnv(assign,value);
  }
    return 0;
}

