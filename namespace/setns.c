#define _GNU_SOURCE
#include <fcntl.h>
#include <sched.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

char *const cmd_args[] = {
"/bin/bash",
NULL
};
 
int main(int argc, char *argv[]) {
	int fd_net, fd_uts;
	char netns[128], utsns[128];
 
	if (argc < 2) {
		printf("%s DOCKER_PID\n", argv[0]);
		exit(0);
	}

	sprintf(netns, "/proc/%s/ns/net", argv[1]);
	fd_net = open(netns, O_RDONLY);
	setns(fd_net, 0);

	sprintf(utsns, "/proc/%s/ns/uts", argv[1]);
	fd_uts = open(utsns, O_RDONLY);
	setns(fd_uts, 0);
	
       // execuate specified command	
	execvp(cmd_args[0], cmd_args);
}
