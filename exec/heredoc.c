/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelhasso <oelhasso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 18:22:21 by oelhasso          #+#    #+#             */
/*   Updated: 2025/09/18 19:44:05 by oelhasso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./header.h"
#include "../minishell.h"


void make_heredoc2(t_cmd *tmp, char *line, t_ind ind)
{
    write (1, "> ", 2);
	line = get_next_line(0);
	if (line == NULL && ind.c == 0)
	{
        close_set(&tmp->pipedoc[READ], -3);
        close_set(&tmp->pipedoc[WRITE], -3);
		if (tmp->flag_exit == 0)
			exit_status(1);
		return (1);
	}
	free(line);
}

int	make_heredoc(t_cmd *tmp, t_other *other, char *limmiter)
{
	char	*line;
	t_ind	ind;

	ind.c = 0;
	while (1)
	{
		make_heredoc2();
		if (line == NULL && ind.c == 1)
			break ;
		ind.c = 1;
		if (is_limiter(line, limmiter) == TRUE)
		{
    	    close_set(tmp->pipedoc[WRITE], 3);
			return (free(line), SUCCESSFUL);
		}
		if (tmp->count_doc == 0 && tmp->red->expand == 0)
    		write (tmp->pipedoc[WRITE], line, mystrlen(line));
    	if (tmp->count_doc == 0 && tmp->red->expand == 1)
    		resolve_heredoc(other->envrp, &line, tmp->pipedoc[WRITE]);
	}
	if (line)
		free(line);
	close_set(&tmp->pipedoc[WRITE], -3);
	close_set(&tmp->pipedoc[READ], -3);
	return (SUCCESSFUL);
}

int child_doc(t_cmd *tmp, t_other *other, t_ind *ind)
{
	t_red *red_copy;

	if (ind->r == ERROR)
	{
		if (tmp->flag_exit == 0)
		{
			restore_fds(other);
			return (exit_status(1), 1);
		}
		restore_fds(other);
		return (close_all_fds(other->orig_cmd), FAILED);
	}
	if (ind->r == SUCCESSFUL)
	{
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
	else
	{
		close(tmp->pipedoc[WRITE]);
		waitpid(ind->r, &other->exit_status, 0);
		handle_exit_status(other->exit_status);
	}
	return (0);
}