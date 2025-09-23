/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelhasso <oelhasso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 18:07:15 by oelhasso          #+#    #+#             */
/*   Updated: 2025/09/23 20:43:20 by oelhasso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./minishell.h"
#include "exec/header.h"

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



int	main(int ac, char **av, char **ev)
{
	char	*buffer;
	t_cmd	*cmd;
	t_env	*env;

	(void)ac;
	(void)av;
	env = handle_env(ev);
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
