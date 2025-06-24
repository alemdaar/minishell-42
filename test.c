#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    // Open the first file (file)
    int fd1 = open("file", O_RDONLY);
    if (fd1 < 0) {
        perror("open file");
        exit(1);
    }

    // Redirect stdin to file
    if (dup2(fd1, STDIN_FILENO) < 0) {
        perror("dup2 file");
        close(fd1);
        exit(1);
    }
    close(fd1);  // no longer needed

    // Open the second file (file1)
    int fd2 = open("file1", O_RDONLY);
    if (fd2 < 0) {
        perror("open file1");
        exit(1);
    }

    // Redirect stdin to file1 (this overrides previous dup2)
    if (dup2(fd2, STDIN_FILENO) < 0) {
        perror("dup2 file1");
        close(fd2);
        exit(1);
    }
    close(fd2);

    // Prepare execve args
    char *argv[] = {"/bin/cat", NULL};
    char *envp[] = {NULL};

    // Call execve — stdin now points to file1
    execve("/bin/cat", argv, envp);

    // If execve fails
    perror("execve");
    exit(1);
}
