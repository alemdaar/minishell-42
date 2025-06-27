#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    char *argv[] = {
        "awk",                 // argv[0] - program name
        "-F",                 // field separator option
        ",",                 // field separator option
        "{print $1}",          // awk script
        "fff",            // input file
        NULL
    };

    char *envp[] = { NULL };  // no special environment variables

    // Full path to awk (you can confirm with `which awk`)
    char *path = "/usr/bin/awk";

    // Execute awk
    if (execve(path, argv, envp) == -1) {
        perror("execve failed");
        return 1;
    }

    return 0;
}
