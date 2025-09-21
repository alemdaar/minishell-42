/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelhasso <oelhasso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 17:26:38 by oelhasso          #+#    #+#             */
/*   Updated: 2025/09/21 20:39:57 by oelhasso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "header.h"

static int is_pipe(t_cmd *cmd, t_other	*other)
{
	other->count_proc = 0;
	while (cmd)
	{
		other->count_proc++;
		cmd = cmd->next;
	}
	if (other->count_proc > 1)
		other->a_pipe = 1;
	else
		other->a_pipe = 0;
	return 0;
}

static void save_fds(t_other *other)
{
	other->stdout_copy = dup(STDOUT_FILENO);
	other->stdin_copy = dup(STDIN_FILENO);
	other->stdin_flag = 0;
	other->stdout_flag = 0;
}

int execution(t_cmd *cmd, t_env *env, char **ev)
{
	t_other	other;

	signal(SIGINT, SIG_IGN);
	other.envr = ev;
	other.envrp = env;
	other.orig_cmd = cmd;
	save_fds(&other);
	is_pipe(cmd, &other);
	edit_paths(&other, env);
	work(cmd, &other);
	close_all_fds(cmd);
	free_all(&other);
	restore_fds(&other);
	return (0);
}
