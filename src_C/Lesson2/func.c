#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>
#include "func.h"

extern char** environ;

void show_info_a_variable (char* en_var){
	if(getenv(en_var)==NULL)
		printf("Variable %s does not exist\n ",en_var );
	else
		printf("%s = %s\n", en_var, getenv(en_var));
}

void infoUsable(){
	printf("environ [--option] [<arg>]\nOptions :\n");
	printf("-h or --help  \t\t\t Print help page\n");
	printf("-i or --info   <variable> 	 Print value of variable\n");
	printf("-s or --set    <variable=value>  Set value to variable\n");
	printf("-a or --assign <variable>   	 and -v or --value <value>  Set <value> to <variable>.\n\t\t\t\t If value not given, set value NULL\n");
	printf("-d or --del    <variable>   	 Remove variable\n");
	printf("-c or --clear  			 Remove all variables\n");
}
void cleanEnv(){
	clearenv();
}
void deleteEnv(char* en_var){
	unsetenv(en_var);
}

void putInEnv(char* en_var,char* value){

	if(en_var != NULL){
		char str[256] = {};
		strcat(str,en_var);
		if(value != NULL){
			if(setenv(str,value,1) != -1){
				printf("%s=%s\n", str, value );
			}
		}
		else{
			if(setenv(str,"NULL",1) != -1){
				printf("%s=NULL\n",str );
			}
		}
		
	}
	else{
		printf("Variable not defined\n");
	}
}

void setValue(char* en_var){

	if(putenv(en_var) == 0){
		printf("Var : %s\n", en_var);
	}
}
// void clearEnv(){
// 	int i;
// 	while(environ[i] != NULL)
// }
