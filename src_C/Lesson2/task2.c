#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

extern char **environ;

void help ();

int main(int argc, char **argv) {
	if(argc == 1) { // если запускаем без аргументов, выводим справку
		for (int i = 0; i < argc; ++i)
		{
			printf("%s\n", *(environ));
		}
		
	return 0;
}
	char* opts = "h:c:o:"; // доступные опции, каждая принимает аргумент
	char* name = NULL;
	int a, b; // тут храним числа
	char op; // а тут оператор
	int opt; // каждая следующая опция попадает сюда
		while((opt = getopt(argc, argv, opts)) != -1) {
			// вызываем getopt пока она не вернет -h1
			switch(opt) {
				case 'h': // если опция -a, преобразуем строку с аргументом в число
					help ();
					break;
				case 'c': // тоже для -b
					name = optarg;

					printf("%s\n", name);
					break;
				case 'o': // в op сохраняем оператор
					
					break;
				case '?':
					printf("%s\n", "Takogo operatora nema" );
					help ();
					continue;

			}
		}
	
	return 0;
}

void help (){
	printf("%s\n", "delaet choto\n");
	printf("%s\n", "-h invoking help\n ");
	printf("%s\n", "-o enter a file name\n");
	printf("%s\n", "-c something");
}

void create_proj (char* argv){

}