#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
 
int main()
{
    int sockfd[2] = {0};
 
    if (-1 == socketpair(PF_UNIX, SOCK_STREAM, 0, sockfd))
    {
        fprintf(stderr, "socketpair: %d, %s\n", errno, strerror(errno));
        exit(1);
    }
 
    pid_t pid = fork();
 
    if (pid < 0)
    {
        fprintf(stderr, "fork: %d, %s\n", errno, strerror(errno));
        exit(1);
    }
    if (pid == 0)
    {
        close(sockfd[1]);
        char buf[] = "data from child program";
        write(sockfd[0], buf, strlen(buf));
        printf("in child program, send data: %s\n", buf);
 
        char szRecv[200] = {0};
        read(sockfd[0], szRecv, 200);
        printf("in child program, get data: %s\n", szRecv);
         
        close(sockfd[0]);
    }
    else
    {
        close(sockfd[0]);
 
        char szRecv[200] = {0};
        read(sockfd[1], szRecv, 200);
        printf("in parent program, get data: %s\n", szRecv);
 
        char buf[] = "data from parent program";
        write(sockfd[1], buf, strlen(buf));
        printf("in parent program, send data: %s\n", buf);
 
        close(sockfd[1]);
        wait(NULL);
    }
     
    return 0;
}
