#include <unistd.h>
#include <sys/types.h>
#include <string.h>
# include <stdio.h>

int main(){
  int fd[2];
  int p;
  char buf[1024];
  
  pipe(fd);

  p = fork();
  if (p == 0){ /* figlio */
    close(fd[0]);
    close(1);
    dup(fd[1]);
    execlp("ls","ls",NULL);
  }
  else if ( p > 0 ) { /* padre */
    int n;
    close(fd[1]);
    while( (n = read(fd[0], buf, 1))  > 0) {
      //buf[0]+=1;
      /*printf("%c", buf[0]);*/
		write(1,buf,n);
    }
    
  }

  return 0;
}
