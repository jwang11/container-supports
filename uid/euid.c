#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(void)
{
    printf(" UID\t= %d\n", getuid());
    printf(" EUID\t= %d\n", geteuid());
    printf(" GID\t= %d\n", getgid());
    printf(" EGID\t= %d\n", getegid());

    return EXIT_SUCCESS;
}
