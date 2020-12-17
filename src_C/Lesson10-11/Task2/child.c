#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <signal.h>
#include <unistd.h>

#define SEM_KEY 2028
#define SHM_KEY "/home/roman/file.log"

union semnum {
    int val;
    struct semid_ds *buf;
    unsigned short * array;
} sem_arg;




int main(int argc, char** argv) {
    key_t token = ftok(SHM_KEY, 1);
    // printf("%d\n", atoi(argv[2]));
    // printf("%s\n", );

    int shm_handler = shmget(token, 128, IPC_CREAT | 0660);
    int sem_handler = semget(SEM_KEY, 1, IPC_CREAT | 0660);
    unsigned short sem_vals[1];
    struct semid_ds ds;
    struct sembuf sb[1];
    if (shm_handler == -1) {
        printf("Child: Can't create shared memory!\n");
        // return 0;
    }
    if (sem_handler == -1) {
        printf("Child: can't create semaphore!\n");
        // return 0;
    }
    int* buffer = shmat(shm_handler, NULL, 0);
    if (buffer == (int*) - 1) {
        printf("Child: Can't get memory address!\n");
    }

    int sum = 0;
    while (1) {
        sum = 0;
        //close semaphore
        sb[0].sem_flg = 0;
        sb[0].sem_num = 0;
        sb[0].sem_op = -1;
        semop(sem_handler, sb, 1);
        int n = buffer[0];
        if (n == -1)
        {
            if (semctl(sem_handler, 0, IPC_RMID, sem_arg) == -1)
            {
                printf("Parent: semctl() error!\n");
                return 0;
            }
            shmctl(shm_handler, IPC_STAT, &ds);
        }
        for (int i = 1; i < n + 1; i++)
            sum += buffer[i];
        buffer[0] = sum;
        sb[0].sem_flg = 0;
        sb[0].sem_num = 0;
        sb[0].sem_op = 1;
        semop(sem_handler, sb, 1);
    }


    return 0;
}
