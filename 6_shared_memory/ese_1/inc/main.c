#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "errExit.h"

#define ARR_SIZE 10

int main (int argc, char *argv[]) {

    struct shared{
        int data[ARR_SIZE];
    };

    int sum = 0;

    int v1[ARR_SIZE] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    int v2[ARR_SIZE] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

    // create the shared memory 
    int shmid = shmget(IPC_PRIVATE, sizeof(struct shared), IPC_CREAT | S_IRUSR | S_IWUSR);
    if (shmid == -1)
        errExit("shmget failed!");
    
    // attach the shared memory segment
    struct shared *ptr_sh = shmat(shmid, 0, 0);
    if (ptr_sh == (void *)-1)
        errExit("shmat failed!");

    pid_t pid;
    for (int i = 0; i < ARR_SIZE; ++i) {

        // generate a subprocess
        pid = fork();
        if (pid == -1){
            printf("child %d not created!\n", i);
        }else if (pid == 0) {
            ptr_sh->data[i] = v1[i] * v2[i];
            printf("<Child %d> %d * %d = %d\n", getpid(), v1[i], v2[i], ptr_sh->data[i]);
            exit(0);
        }
    }

    int status = 0;

    // wait for all the child to terminate before adding the results.
    while ( (pid = wait(&status)) != -1);

    // sum the results
    for(int i=0; i < ARR_SIZE; i++) {
        sum += ptr_sh->data[i];
    }
    
    printf("The result is: %d\n", sum);

    // remove the shared memory segment
    if (shmdt(ptr_sh) == -1)
        errExit("shmget failed!");

    return 0;
}