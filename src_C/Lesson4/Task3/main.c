#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(void) {
    int variable_for_fork = 0;
    for (int i = 0; i < 2; ++i)        
    {   sleep(3);
        pid_t pid = fork();

        switch(pid) {
            case -1:
                perror("fork");
                return -1;
            case 0:
                // Child  
                sleep(1);  
                pid_t pidch = fork();
                pidch = fork();
                if (pidch == 0)
                {   
                    sleep(1); 
                    printf("Child in child my pid = %i, parent pid = %i, local var = %i\n",
                    getpid(), getppid(), variable_for_fork+=1);
                }
                else if(pidch<0)
                    perror("fork");
                else
                {    
                    sleep(1); 
                    printf("Parent in child my pid = %i, parent pid = %i, local var = %i\n",
                    getpid(), getppid(), variable_for_fork+=1);
                }
                // printf("Child my pid = %i, parent pid = %i\n", getpid(), getppid());
                break;
            default:
                // Parent
                sleep(1); 
                printf(" Main Parent my pid = %i, parent pid = %i, local var = %i\n",
                getpid(), getppid(), variable_for_fork+=1);
                break;
        }
    }
    
   
    return EXIT_SUCCESS;
}