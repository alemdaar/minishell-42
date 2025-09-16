/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelhasso <oelhasso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 18:07:15 by oelhasso          #+#    #+#             */
/*   Updated: 2025/09/16 22:17:32 by oelhasso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./minishell.h"
#include "exec/header.h"

int	exit_status(int val)
{
	static int	exit;

	if (val > -1)
		exit = val;
	return (exit);
}

void    handle_exit_status(int status)
{
    if (WIFSIGNALED(status))
    {
        if (WTERMSIG(status) == SIGINT)
            exit_status(130);
        else if (WTERMSIG(status) == SIGQUIT)
        {
            exit_status(131);
            write(1, "Quit: 3\n", 8);
        }
    }
    else if (WIFEXITED(status))
        exit_status(WEXITSTATUS(status));
}

static char	*read_line(t_env *env)
{
	char	*read_line;

	read_line = NULL;
	read_line = readline("minishell-$> ");
	if (!read_line)
		return (clean_env(env), printf("exit\n"), exit(0), NULL);
	if (read_line[0] == '\0')
		return (read_line);
	add_history(read_line);
	return (read_line);
}

void	handle_sigint(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	exit_status(130);
}

void	set_signals_interactive(void)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
}

void	set_signals_in_child(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

int	main(int ac, char **av, char **ev)
{
	char	*buffer;
	t_cmd	*cmd;
	t_env	*env;

	(void)ac;
	(void)av;
	env = handle_env(ev);
	// set_signals_interactive();
	while (1)
	{
		rl_catch_signals = 0;
		set_signals_main();
		buffer = read_line(env);
		cmd = parcing(buffer, env);
		if (cmd)
			execution(cmd, env, ev);
	}
}
