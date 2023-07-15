#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

#include "errExit.h"

#define PIPE_SYMBOL "|"

int main (int argc, char *argv[]) {
	
    /* Validates the usage. At least is needed the program's name, two commands and the pipe symbol */
    if ( argc < 3 ) {
        printf("usage: %s command-1 [args-1...] | command-2 [args-2...]\n", argv[0]);
        exit(0);
    }

    /* The start of the first comment is allways the start of the program arguments array */
    char **command1 = &argv[1];

    /* The start of the second command is undefined, once it depends where the pipe symbol is located */
    char **command2 = NULL;

    /* Finds the position of the pipe symbol */
    for ( int i = 0 ; argv[i] != NULL ; i++ ) {
        /* When found, ... */ 
        if ( strcmp(PIPE_SYMBOL, argv[i]) == 0 ) {
            /* ... replaces it for NULL, so the first command array is NULL terminated and... */
            argv[i] = NULL;
            /* ... the next position is the start of the second command */
            command2 = &argv[i+1];
            break;
        }
    }
	
    /* If the pipe symbol is missing or if there is no command after the pipe symbol, bad usage */
    if ( command2 == NULL || command2[0] == NULL ) {
        printf("usage: %s command-1 [args-1...] | command-2 [args-2...]\n", argv[0]);
        exit(0);
    }

    pid_t pid1, pid2;
    int fd[2];

    if ( pipe(fd) == -1 ) {
		errExit("creating pipe");
    }

	/* 1st child process executes the first command */
    if ( (pid1 = fork()) == -1 ) {
		errExit("creating child process");
    }
    
    if ( pid1 == 0 ) {
		dup2(fd[1], STDOUT_FILENO);
		close(fd[0]);
		close(fd[1]);
        if(execvp(command1[0], command1) < 0) {
        	errExit("executing first command");
        }		
    }
	
	/* 2nd child process executes the first command */
    if ( (pid2 = fork()) == -1 ) {
		errExit("creating child process");
    }
    
    if ( pid2 == 0 ) {
		dup2(fd[0], STDIN_FILENO); // waiting to read from the pipe
		close(fd[0]);
		close(fd[1]);
        if(execvp(command2[0], command2) < 0) {
        	errExit("executing first command");
        }		
    }
	
	close(fd[0]);
	close(fd[1]);
	
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
	
	return 0;
}