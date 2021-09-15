#define _GNU_SOURCE
#include <sched.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

char *const cmd_args[] = {
"/bin/bash",
NULL
};
 
#define errExit(msg)    do { perror(msg); exit(EXIT_FAILURE); } while (0)

int main(int argc, char *argv[])
{
    if (unshare(CLONE_NEWNET|CLONE_NEWUTS) == -1)
        errExit("unshare");

    execvp(cmd_args[0], cmd_args);
}
