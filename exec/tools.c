/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelhasso <oelhasso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 12:42:53 by oelhasso          #+#    #+#             */
/*   Updated: 2025/09/18 12:59:21 by oelhasso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "./header.h"

int is_builtin(t_cmd *tmp, t_other *other)
{
	if (is_equal(tmp->commands[0], "echo"))
		return (tmp->bin = 1, SUCCESSFUL);
	if (is_equal(tmp->commands[0], "cd"))
		return (tmp->bin = 1, SUCCESSFUL);
	if (is_equal(tmp->commands[0], "pwd"))
		return (tmp->bin = 1, SUCCESSFUL);
	if (is_equal(tmp->commands[0], "export"))
		return (tmp->bin = 1, SUCCESSFUL);
	if (is_equal(tmp->commands[0], "unset"))
		return (tmp->bin = 1, SUCCESSFUL);
	if (is_equal(tmp->commands[0], "env"))
		return (tmp->bin = 1, SUCCESSFUL);
	if (is_equal(tmp->commands[0], "exit"))
		return (tmp->bin = 1, SUCCESSFUL);
	return (FAILED);
}

int count_heredoc(t_cmd *tmp)
{
	t_red	*copy;

	copy = tmp->red;
	while (copy)
	{
		if (copy->red_type == HERDOOC)
		{
			tmp->limiter = copy->file;
			tmp->count_doc ++;
		}
		copy = copy->next;
	}
	return (tmp->count_doc);
}

void count_args(t_cmd *tmp)
{
	int i = 0;
	while (tmp->commands[i])
		i++;
	tmp->ar = i;
	return ;
}

int	set_up(t_cmd *tmp)
{
	tmp->argument = NULL;
	tmp->path_cmd = NULL;
	tmp->limiter = NULL;
	tmp->open1 = -3;
	tmp->open2 = -3;
	tmp->pipefd[WRITE] = -3;
	tmp->pipefd[READ] = -3;
	tmp->pipedoc[WRITE] = -3;
	tmp->pipedoc[READ] = -3;
	tmp->is_limiter = 0;
	tmp->ar = 1;
	tmp->bin = 0;
	tmp->count_doc = 0;
	tmp->flag_exit = 0;
	return (SUCCESSFUL);
}
