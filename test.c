#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <program> [args...]\n", argv[0]);
        return 1;
    }

    // argv[1] is the program, argv[2..] are its arguments
    char **cmd_args = &argv[1];

    // Execute the command
    if (execve(cmd_args[0], cmd_args, NULL) == -1) {
        perror("execve failed");
        return 1;
    }

    // This line is never reached if execve succeeds
    return 0;
}
