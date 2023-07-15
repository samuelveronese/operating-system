#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

#include "errExit.h"

#define PIPE_SYMBOL "|"

int main (int argc, char *argv[]) {
	
	int fd1[2];
	int fd2[2];
	
    pid_t pid1, pid2, pid3;
	
    if ( pipe(fd1) == -1 ) {
		errExit("creating pipe1");
    }
     
	/* 1st child process executes the first command */
    if ( (pid1 = fork()) == -1 ) {
		errExit("creating child process");
    }
    
    if ( pid1 == 0 ) {
		dup2(fd1[1], STDOUT_FILENO);
		
		close(fd1[0]);
		close(fd1[1]);
        execlp("ls", "ls", "-al", NULL);
        errExit("executing first command");	
    }
	
    if ( pipe(fd2) == -1 ) {
		errExit("creating pipe1");
    }
	
	/* 2nd child process executes the first command */
    if ( (pid2 = fork()) == -1 ) {
		errExit("creating child process");
    }
    
    if ( pid2 == 0 ) {
		dup2(fd1[0], STDIN_FILENO); // waiting to read from the pipe
		dup2(fd2[1], STDOUT_FILENO);		
		close(fd1[0]);
		close(fd1[1]);
		close(fd2[0]);
		close(fd2[1]);
        execlp("cut", "cut", "-b27-", NULL);
        errExit("executing second command");		
    }
	
	close(fd1[0]);
	close(fd1[1]);
	
	/* 3rd child process executes the first command */
    if ( (pid3 = fork()) == -1 ) {
		errExit("creating child process");
    }
    
    if ( pid3 == 0 ) {
		dup2(fd2[0], STDIN_FILENO); // waiting to read from the pipe
		close(fd2[0]);
		close(fd2[1]);
        if(execlp("sort", "sort", "-n", NULL) == -1) {
        	errExit("executing third command");
        }		
    }
	
	close(fd2[0]);
	close(fd2[1]);
	
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
	waitpid(pid3, NULL, 0);
	
	return 0;
}