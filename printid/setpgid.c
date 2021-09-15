#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() 
{
  pid_t pid;
  int wstat;

  if ((pid=fork())<0) 
  {
    printf("fork error!");
    exit(1);
  }
  else if (pid==0) 
  {//child process
    printf("The child process PID is %d.\n",getpid());
    printf("The Group ID of child is %d.\n",getpgid(0)); // 返回组id
    sleep(5);
    printf("The Group ID of child is changed to %d.\n",getpgid(0));
    exit(0);
  }
  
  sleep(1);
  /*
  1. parent process:
  will first execute the code blow
  对于父进程来说，它的PID就是进程组ID: PGID，所以setpgid对父进程来说没有变化
  
  2. child process 
  will also execute the code blow after 5 seconds
  对于子进程来说，setpgid将改变子进程所属的进程组ID
  */
  // 改变子进程的组id为子进程本身
  setpgid(pid, pid); 
  
  waitpid(pid, &wstat, 0);
  printf("\n");
  printf("The process PID is %d.\n",getpid()); 
  printf("The Group ID of Process is %d.\n",getpgid(0)); 

  return 0;
}
