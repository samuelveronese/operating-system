#include <unistd.h>
#include <sys/types.h>
#include <string.h>

int main(){
  int fd[2];
  int p;
  char buf[1024];
  
  pipe(fd);

  p = fork();
  if (p == 0){ /* figlio */
    char st[] = "Ciao io sono il giglio\n";
    close(fd[0]);
    sleep(3);
    write(fd[1], st, strlen(st));
  }
  else if ( p > 0 ) { /* padre */
    int n;
    /* close(fd[1]); */
    while( (n = read(fd[0], buf, 2))  > 0)
      write(1,buf,n);

  }

  return 0;
}
