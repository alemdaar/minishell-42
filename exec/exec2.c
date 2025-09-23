/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelhasso <oelhasso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 16:56:14 by oelhasso          #+#    #+#             */
/*   Updated: 2025/09/23 21:52:05 by oelhasso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "./header.h"
#include "gnl/get_next_line.h"

static int close_parent(t_cmd *tmp, t_other *other, int i)
{
	if (i == 0 && other->a_pipe)
		close_set(&tmp->pipefd[WRITE], -3);
	else if (tmp->next && i != 0)
	{
		if (tmp->pipefd[WRITE])
			close_set(&tmp->pipefd[WRITE], -3);
		if (tmp->prev->pipefd[READ])
			close_set(&tmp->prev->pipefd[READ], -3);
	}
	else if (tmp->next == NULL && other->a_pipe)
	{
		if (tmp->prev->pipefd[READ])
			close_set(&tmp->prev->pipefd[READ], -3);
	}
	if (tmp->pipedoc[READ] != -3)
		close_set(&tmp->pipedoc[READ], -3);
	if (tmp->pipedoc[WRITE] != -3)
		close_set(&tmp->pipedoc[WRITE], -3);
	return (0);
}

static void check_directory(t_cmd *tmp, t_other *other)
{
    if (ft_strchr(tmp->commands[0], '/') != -1)
	{
		if (stat(tmp->commands[0], &st) == -1)
		{
            print_err("", tmp->commands[0], NO_DIR);
			exit(127);
		}
		if (S_ISDIR(st.st_mode))
		{
            print_err("", tmp->commands[0], IS_DIR);
			exit(126);
		}
		if (access(tmp->commands[0], X_OK) == -1)
		{
            print_err("", tmp->commands[0], NO_PERM);
			exit(126); 
		}
		execve(tmp->commands[0], tmp->argument, other->envr);
		perror("minishell");
		exit(126); 
	}
}
   
int	exec(t_cmd *tmp, t_other *other)
{
	int r;

	if (tmp->bin == 1)
		return (r = run_bin(tmp, other), r);
	if (!tmp->commands[0] || tmp->commands[0][0] == '\0')
	{
	    restore_fds(other);
	    fprintf (stderr, "minishell: : command not found\n");
	    return (free_all(other), 127);
	}
    check_directory(tmp, other);
	if (tmp->path_cmd == NULL) 
	{
        fprintf (stderr, "minishell: %s: command not found\n", tmp->commands[0]);
		restore_fds(other);
		return (free_all(other), 127);
	}
	execve(tmp->path_cmd, tmp->argument, other->envr);
	perror("minishell");
	exit(126);
	return (SUCCESSFUL);
}

int	execution2(t_cmd *tmp, t_other *other, int i)
{
	t_ind ind;

	if (tmp->pid == -1)
	{
		perror ("minishel: fork: ");
		return (ERROR);
	}
	else if (tmp->pid == 0)
	{
		set_signals_child();
		restore_fds(other);
		ind.r = child_process(tmp, other, i);
		close_all_fds(other->orig_cmd);
		exit (ind.r);
	}
	close_parent(tmp, other, i);
	return (SUCCESSFUL);
}