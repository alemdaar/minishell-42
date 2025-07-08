#include <unistd.h>
#include <stdio.h>

int main(void)
{
    char buffer[1024];

    if (chdir("pppppp") == -1)
        perror("cd");
    else
        printf("Changed to /tmp successfully\n");

    // Show the new working directory of this process
    if (getcwd(buffer, sizeof(buffer)))
        printf("Current directory: %s\n", buffer);
    else
        perror("getcwd");

    return 0;
}
