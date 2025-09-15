#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>

static void sigint_handler(int sig)
{
    (void)sig;
    write(1, "\n", 1);          // go to new line
    rl_replace_line("", 0);      // clear current input
    rl_on_new_line();            // move cursor to new line
    rl_redisplay();              // redraw prompt
}

static void sigquit_handler(int sig)
{
    (void)sig;
    // Do nothing (ignore Ctrl+\)
}

int main(void)
{
    char *line;

    // Set signal handlers
    signal(SIGINT, sigint_handler);
    signal(SIGQUIT, sigquit_handler);

    while (1)
    {
        line = readline("minishell-$> ");
        if (!line) // Ctrl+D
        {
            printf("exit\n");
            break;
        }
        if (*line)
            add_history(line);
        free(line);
    }
    return 0;
}
