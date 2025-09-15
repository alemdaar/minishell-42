/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelhasso <oelhasso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 18:07:15 by oelhasso          #+#    #+#             */
/*   Updated: 2025/09/15 19:51:33 by oelhasso         ###   ########.fr       */
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
	{
		clean_env(env);
		printf("exit\n");
		exit(0);
	}
	history_input(read_line);
	return (read_line);
}

int	main(int ac, char **av, char **ev)
{
	char	*buffer;
	t_cmd	*cmd;
	t_env	*env;

	(void)ac;
	(void)av;
	env = construct_env(ev);
	while (1)
	{
		// rl_catch_signals = 0;
		// set_signals_main();
		// while (var);
		buffer = read_line(env);
		cmd = assemble_command(buffer, env);
		if (!cmd)
			continue ;
		execution(cmd, env, ev);
	}
}
