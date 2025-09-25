/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelhasso <oelhasso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 18:22:21 by oelhasso          #+#    #+#             */
/*   Updated: 2025/09/25 15:21:44 by oelhasso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./header.h"
#include "../minishell.h"
#include "./gnl/get_next_line.h"

static void	make_heredoc3(char *line, t_cmd *tmp)
{
	if (line)
		free(line);
	close_set(&tmp->pipedoc[WRITE], -3);
	close_set(&tmp->pipedoc[READ], -3);
	return ;
}

static char	*make_heredoc2(t_cmd *tmp, t_ind ind)
{
	char	*line;

	write (1, "> ", 2);
	line = get_next_line(0);
	if (line == NULL && ind.c == 0)
	{
		close_set(&tmp->pipedoc[READ], -3);
		close_set(&tmp->pipedoc[WRITE], -3);
		if (tmp->flag_exit == 0)
			exit_status(1);
		return (NULL);
	}
	return (line);
}

static int	make_heredoc(t_cmd *tmp, t_other *other, char *limmiter)
{
	char	*line;
	t_ind	ind;

	ind.c = 0;
	while (1)
	{
		line = make_heredoc2(tmp, ind);
		if (line == NULL)
			return (1);
		if (line == NULL && ind.c == 1)
			break ;
		ind.c = 1;
		if (is_limiter(line, limmiter) == TRUE)
		{
			close_set(&tmp->pipedoc[WRITE], 3);
			return (free(line), SUCCESSFUL);
		}
		if (tmp->count_doc == 0 && tmp->red->expand == 0)
			write (tmp->pipedoc[WRITE], line, mystrlen(line));
		if (tmp->count_doc == 0 && tmp->red->expand == 1)
			resolve_heredoc(other->envrp, &line, tmp->pipedoc[WRITE]);
	}
	make_heredoc3(line, tmp);
	return (SUCCESSFUL);
}

static int	child_doc2(t_other *other, t_cmd *tmp)
{
	t_red	*red_copy;

	set_signals_heredoc();
	restore_fds(other);
	red_copy = tmp->red;
	while (red_copy)
	{
		if (red_copy->red_type == HERDOOC)
		{
			tmp->count_doc --;
			make_heredoc(tmp, other, red_copy->file);
		}
		red_copy = red_copy->next;
	}
	exit (0);
}

int	child_doc(t_cmd *tmp, t_other *other, t_ind *ind)
{
	if (ind->r == ERROR)
	{
		if (tmp->flag_exit == 0)
			return (restore_fds(other), exit_status(1), 1);
		restore_fds(other);
		return (close_all_fds(other->orig_cmd), FAILED);
	}
	if (ind->r == SUCCESSFUL)
		child_doc2(other, tmp);
	else
	{
		close(tmp->pipedoc[WRITE]);
		waitpid(ind->r, &other->exit_status, 0);
		handle_exit_status(other->exit_status);
	}
	return (0);
}
