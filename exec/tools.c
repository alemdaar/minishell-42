/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelhasso <oelhasso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 12:42:53 by oelhasso          #+#    #+#             */
/*   Updated: 2025/09/18 22:48:23 by oelhasso         ###   ########.fr       */
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

void print_err(char *command, char *subject, char *msg)
{
	fprintf (stderr, "minishell: %s: %s: %s", command, subject, msg);
	return ;
}

int	is_limiter(char *line, char *limiter)
{
	t_ind	ind;

	ind.i = 0;
	// printf("limiter : %s\n", limiter);
	// printf("line : %s\n", line);
	while (limiter[ind.i] && limiter[ind.i] == line[ind.i])
		ind.i ++;
	if (limiter[ind.i] == 0 && line[ind.i] == '\n')
		return (TRUE);
	return (FALSE);
}
int	is_equal(char *command, char *b_in)
{
	int i = 0;
	while (command[i] && b_in[i] && b_in[i] == command[i])
		i++;
	if (b_in[i] == command[i])
		return (1);
	return (0);
}

size_t mystrlen(char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

void why_exit(char *str, int flag)
{
	if (flag == SUCCESSFUL)
		perror (str);
	else if (flag == FAILED)
		printf ("%s", str);
	exit_status(flag);
	exit(flag);
}
