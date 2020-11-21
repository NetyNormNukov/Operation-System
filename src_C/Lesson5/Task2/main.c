#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
struct thread_arg {
	char* name;
	char * str;
	int num;
};
void * any_func(void * arg) {
	struct thread_arg targ =
	    *(struct thread_arg *) arg;
	    for (int i = 0; i < targ.num; ++i)
	    {
	    	printf("Thread %s.%s %d\n", targ.name, targ.str, i+1);
	    }
	
	return NULL;
}
int main(void) {
	setbuf(stdout, NULL);
	pthread_t thread;
	struct thread_arg targ[] = {{"nameThread1","str1",1},{"nameThread2","str2",2},{"nameThread3","str3",3},{"nameThread4","str4",4}};

	for (int i = 0; i < 3; ++i)
	{
		if(pthread_create(&thread, NULL, &any_func, &targ[i]) != 0)
		fprintf(stderr, "Error in thread \n");
	}
	sleep(3);
	return EXIT_SUCCESS;
}
