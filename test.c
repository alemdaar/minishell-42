#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid;
    int status;

    pid = fork();
    if (pid == 0)
    {
        char *argv[] = {"ls", "-l", NULL};
        return (15); // Make sure child exits if execve fails
    }
    else if (pid > 0)
    {
        // Parent process
        waitpid(pid, &status, 0);
        while (1);
        // Handle exit status
        if (WIFEXITED(status)) {
            int code = WEXITSTATUS(status);
            printf("Command exited with status: %d\n", code);
        } else if (WIFSIGNALED(status)) {
            int sig = WTERMSIG(status);
            printf("Command was terminated by signal: %d\n", sig);
        }
    } else {
        perror("fork failed");
    }

    return 123;
}
