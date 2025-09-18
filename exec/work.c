/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   work.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelhasso <oelhasso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 11:49:40 by oelhasso          #+#    #+#             */
/*   Updated: 2025/09/18 14:52:36 by oelhasso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "./header.h"

int prepare_doc(t_cmd *cmd, t_other *other, t_ind ind)
{
    if (ind.r > 0)
    {
        ind.r = pipping(cmd, 2);
        if (ind.r == ERROR)
        {
            restore_fds(other);
            if (cmd->flag_exit == 0)
                return (exit_status(1), 1);
            return (1);
        }
        ind.r = fork ();
        ind.c = child_doc(cmd, other, &ind);
        if (ind.c == FAILED)
            return (FAILED);
    }
}

int prepare_exec(t_cmd *cmd, t_other *other, t_ind ind)
{
	while (cmd)
	{
		set_up(cmd);
		ind.r = count_heredoc(cmd);
        if (prepare_doc(cmd, other, ind) == 1)
            return (1);
		if (cmd->commands[0])
		{
			ind.c = check_cmd(cmd, other);
			if (ind.c == ERROR)
			{
				restore_fds(other);
				if (cmd->flag_exit == 0)
				    exit_status(1);
				return (close_all_fds(other->orig_cmd), 1);
			}
			count_args(cmd);
			ind.r = fill_argument(cmd, other);
			if (ind.r == ERROR)
			{
				restore_fds(other);
				return (exit_status(1), close_all_fds(other->orig_cmd), 1);
			}
		}
		cmd = cmd->next;
	}
}

int work3(t_ind ind, t_other *other)
{
	while (ind.i--)
	{
		ind.r = wait(&other->exit_status);
		if (ind.r == ind.c)
			handle_exit_status(other->exit_status);
	}
}

int work2(t_cmd *tmp, t_other *other, t_ind ind)
{
	ind.i = 0;
	while (tmp)
	{
		if (tmp->next)
		{
			ind.r = pipping(tmp, 1);
			if (ind.r == ERROR)
			{
				restore_fds(other);
				exit_status(1);
				return (close_all_fds(other->orig_cmd), 1);
			}
		}
		tmp->pid = fork();
		ind.c = tmp->pid;
		ind.r = execution2(tmp, other, ind.i);
		if (ind.r == ERROR)
		{
			restore_fds(other);
			return (0);
		}
		ind.i++;
		tmp = tmp->next;
	}
	work3(ind, other);
	return (SUCCESSFUL);
}

int  work(t_cmd *cmd, t_other *other)
{
	t_cmd	*tmp;
	t_ind	ind;

    if (prepare_exec(cmd, other, ind) == 1)
        return (1);
	tmp = cmd;
	if (cmd->next == NULL && cmd->bin == 1)
	{
		ind.r = child_process(cmd, other, 0);
		if (ind.r == 1)
		{
			restore_fds(other);
			if (tmp->flag_exit == 0)
				exit_status(1);
			return (1);
		}
	}
	else
	{
		ind.r = work2(tmp, other, ind);
		if (ind.r == 1)
		{
			restore_fds(other);
			return (close_all_fds(cmd), 1);
		}
	}
	return (SUCCESSFUL);
}
