#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int ac, char **av, char **env)
{
    (void)ac;
    (void)av;
    char *args[] = {"/usr/bin/which", "ls", NULL};
    execve("/usr/bin/which", args, env);

    // If execve fails
    perror("execve failed");
    return 1;
}
