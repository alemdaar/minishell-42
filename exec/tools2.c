/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelhasso <oelhasso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 21:58:21 by oelhasso          #+#    #+#             */
/*   Updated: 2025/09/21 21:58:50 by oelhasso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "./header.h"

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

int	pipping(t_cmd *tmp, int type)
{
	t_ind	ind;

	if (type == 1)
		ind.r = pipe(tmp->pipefd);
	else
		ind.r = pipe(tmp->pipedoc);
	if (ind.r == -1)
	{
		perror ("Error: pipe failed\n");
		return (ERROR);
	}
	return (SUCCESSFUL);
}

void close_set(int *to_be_closed, int value)
{
	close(*to_be_closed);
	*to_be_closed = value;
	return ;
}
