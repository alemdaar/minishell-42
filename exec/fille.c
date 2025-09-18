/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fille.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelhasso <oelhasso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 12:18:28 by oelhasso          #+#    #+#             */
/*   Updated: 2025/09/18 12:38:19 by oelhasso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "./header.h"

int file_pipe (t_cmd *tmp, int flag)
{
    if (flag == 0)
    {
    	tmp->open2 = tmp->pipefd[WRITE];
    	close(tmp->pipefd[READ]);
    }
    else if (tmp->next == NULL)
    	tmp->open1 = tmp->prev->pipefd[READ];
    else
    {
    	tmp->open1 = tmp->prev->pipefd[READ];
    	tmp->open2 = tmp->pipefd[WRITE];
    	close(tmp->pipefd[READ]);
    }
}
int heredoc_case(t_cmd *tmp, t_other *other)
{
    if (tmp->pipefd[READ] != -3  && other->a_pipe)
	{
		close(tmp->pipefd[READ]);
		tmp->pipefd[READ] = -3;
	}
	if (tmp->open1 != -3)
	{
		close(tmp->open1);
		tmp->open1 = -3;
	}
	tmp->open1 = tmp->pipedoc[READ];
}

int redout_case(t_cmd *tmp, t_other *other, t_red *copy_red)
{
	if (tmp->pipefd[WRITE] != -3 && other->a_pipe)
	{
		close(tmp->pipefd[WRITE]);
		tmp->pipefd[WRITE] = -3;
	}
	if (tmp->open2 != -3)
	{
		close (tmp->open2);
		tmp->open2 = -3;
	}
	tmp->open2 = open (copy_red->file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (tmp->open2 == -1)
    {
        fprintf(stderr, "minishell: %s: ", copy_red->file);
		return (perror(""), -1);
    }
}

int append_case(t_cmd *tmp, t_other *other, t_red *copy_red)
{
    if (tmp->pipefd[WRITE] != -3 && other->a_pipe)
    {
    	close(tmp->pipefd[WRITE]);
    	tmp->pipefd[WRITE] = -3;
    }
    if (tmp->open2 != -3)
    {
    	close (tmp->open2);
    	tmp->open2 = -3;
    }
    tmp->open2 = open (copy_red->file, O_CREAT | O_WRONLY | O_APPEND, 0644);
    if (tmp->open2 == -1)
    {
    	fprintf(stderr, "minishell: %s: ", copy_red->file);
        return (perror(""), -1);
    }
}

int redin_case(t_cmd *tmp ,t_other *other, t_red *copy_red)
{
    if (tmp->pipefd[READ] != -3 && other->a_pipe)
	{
		close(tmp->pipefd[READ]);
		tmp->pipefd[READ] = -3;
	}
	if (tmp->open1 != -3)
	{
		close (tmp->open1);
		tmp->open1 = -3;
	}
	tmp->open1 = open (copy_red->file, O_RDONLY);
	if (tmp->open1 == -1)
	{
        fprintf(stderr, "minishell: %s: ", copy_red->file);
		return (perror(""), -1);
	}
}

int	check_file(t_cmd *tmp, t_other *other, int flag)
{
	t_red	*copy_red;

	if (other->a_pipe)
		file_pipe (tmp, flag);
	copy_red = tmp->red;
	while (copy_red)
	{
		if (copy_red->red_type == HERDOOC)
			heredoc_case(tmp, other);
		else if (copy_red->red_type == REDIR_OUT)
            redout_case(tmp ,other, copy_red);
		else if (copy_red->red_type == APPEND)
            append_case(tmp, other, copy_red);
		else if (copy_red->red_type == REDIR_IN)
            redin_case(tmp ,other, copy_red);
		copy_red = copy_red->next;
	}
	return (SUCCESSFUL);
}