#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>

#define ARR_SIZE 10

int main (int argc, char *argv[]) {

    struct shared{
        int data[ARR_SIZE];
    };

    int sum = 0;

    int v1[ARR_SIZE] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    int v2[ARR_SIZE] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

    //Parent process create the shared memory 
    int shmid = shmget(IPC_PRIVATE, sizeof(struct shared), IPC_CREAT | S_IRUSR | S_IWUSR);
    if (shmid == -1){
        printf("shmget failed!\n");
        exit(1);
        }


    pid_t pid;
    
    struct shared *ptr_sh = shmat(shmid, 0, 0);
    if (ptr_sh == (void *)-1){
        printf("shmat failed");
        exit(1);
    }

    for (int i = 0; i < ARR_SIZE; ++i) {

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
    // get termination status of each created subprocess.
    while ( (pid = wait(&status)) != -1);
        //printf("Child %d exited\n", pid);

    for(int i=0; i < ARR_SIZE; i++) {
        sum += ptr_sh->data[i];
    }

    if (shmdt(ptr_sh) == -1){
        printf("shmget failed!\n");
        exit(1);
    }

    printf("The result is: %d\n", sum);

    return 0;
}