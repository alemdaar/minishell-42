#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    char *args[] = {"/usr/bin/yes", NULL};   // command + arguments
    char *envp[] = {NULL};                    // environment (can be inherited or empty)

    if (execve(args[0], args, envp) == -1) {
        perror("execve failed");
        exit(EXIT_FAILURE);
    }

    // This will never run unless execve fails
    return 0;
}
