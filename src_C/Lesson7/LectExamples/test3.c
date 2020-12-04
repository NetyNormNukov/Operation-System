//This program demonstrates the deadlock
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t lock1, lock2;

void *thread1(void * arg){
    int i;
    int num = *(int*)arg;
    for (i = 0; i < num; i++) {
        pthread_mutex_lock(&lock1);
        printf("Job started in resource 1 (%d)...\n", i+1);
        //sleep(2);
        printf("Trying to get resource 2\n");
        pthread_mutex_lock(&lock2); 
        printf("Aquired resource 2\n");
        pthread_mutex_unlock(&lock2);
        printf("Job finished in resource 1(%d)...\n", i+1);
        pthread_mutex_unlock(&lock1);
    }
    return NULL;
}

void *thread2(void * arg){
    int num = *(int*)arg;
    int i;
    for (i = 0; i < num; i++) {
        pthread_mutex_lock(&lock2);
        printf("\tJob started in resource 2 (%d)...\n", i+1);
        //sleep(2);
        printf("\tTrying to get resource 1\n");
        pthread_mutex_lock(&lock1); 
        printf("\tAquired resource 1\n");
        pthread_mutex_unlock(&lock1);
        printf("\tJob finished in resource 2 (%d)...\n", i+1);
        pthread_mutex_unlock(&lock2);
    }   
    return NULL;
}



int main(int argc, char **argv) {
    int num = 10;
    pthread_t tid1,tid2;

    if (argc == 2) {
        num = atoi(argv[1]);
    }

    setbuf(stdout, NULL);
    pthread_mutex_init(&lock1, NULL);
    pthread_mutex_init(&lock2, NULL);

    pthread_create(&tid1, NULL, thread1, &num);
    pthread_create(&tid2, NULL, thread2, &num);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    return EXIT_SUCCESS;;

}