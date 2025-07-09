#include <unistd.h>
#include <fcntl.h>

// 1. Save original stdout
int stdout_copy = dup(STDOUT_FILENO);

// 2. Redirect stdout to a file
int fd = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
dup2(fd, STDOUT_FILENO);
close(fd); // optional but recommended after dup2

// 3. Execute the built-in that writes to stdout
printf("this goes to file\n");

// 4. Restore original stdout
dup2(stdout_copy, STDOUT_FILENO);
close(stdout_copy);

// 5. Now stdout goes back to terminal
printf("this goes to terminal\n");
