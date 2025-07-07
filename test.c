#include <unistd.h>
#include <stdio.h>

int main(void)
{
    // Try to change to a directory that exists
    if (chdir("/tmp") == -1)
        perror("cd");
    else
        printf("Changed to /tmp successfully\n");
    return 0;
}
