/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelhasso <oelhasso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 17:26:38 by oelhasso          #+#    #+#             */
/*   Updated: 2025/06/24 09:51:13 by oelhasso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "header.h"
#include "get_next_line.h"

// void test(t_cmd *cmd)
// {
// 	int i;

// 	i = 0;
// 	while (cmd)
// 	{
// 		printf ("1---\n");
// 		i = 0;
// 		while (cmd->commands[i])
// 			printf ("cmd : %s\n", cmd->commands[i++]);
// 		while (cmd->red)
// 		{
// 			printf ("file : %s\n", cmd->red->file);
// 			printf ("type : %u\n", cmd->red->red_type);
// 			cmd->red = cmd->red->next;
// 		}
// 		cmd = cmd->next;
// 		printf ("2---\n");
// 	}
// }

int	exec(t_cmd *tmp, t_cmd *cmd, t_other *other)
{
	if (tmp->path_cmd == NULL)
	{
		if (access("/tmp/here_doc", F_OK) == 0)
			unlink("/tmp/here_doc");
		free_all(&cmd, other);
		myputstr ("Error: command not found\n", 2);
		exit(1);
	}
	if (execve(tmp->path_cmd, tmp->argument, NULL) == ERROR)
	{
		if (access("/tmp/here_doc", F_OK) == 0)
			unlink("/tmp/here_doc");
		free_all(&cmd, other);
		perror ("execve: ");
		exit(1);
	}
	return (SUCCESSFUL);
}

int	dupping2(t_cmd *tmp, t_other *other, int position)
{
	t_ind	ind;

	if (position != 0 && tmp->next != NULL)
	{
		ind.r = dup2(other->prev_read, 0);
		if (ind.r == -1)
			return (close_fds(tmp->pipefd, other->prev_read), ERROR);
		ind.r = dup2(tmp->pipefd[WRITE], 1);
		if (ind.r == -1)
			return (close_fds(tmp->pipefd, other->prev_read), ERROR);
		close_fds(tmp->pipefd, other->prev_read);
	}
	else
	{
		ind.r = dup2(other->prev_read, 0);
		if (ind.r == -1)
			return (close(other->prev_read), close(other->open2), ERROR);
		ind.r = dup2(other->open2, 1);
		if (ind.r == -1)
			return (close(other->prev_read), close(other->open2), ERROR);
		close(other->prev_read);
		close(other->open2);
	}
	return (SUCCESSFUL);
}

int	dupping(t_cmd *tmp, t_other *other, int position)
{
	t_ind	ind;

	if (position == 0 && tmp->next != NULL)
	{
		ind.r = dup2(other->open1, 0);
		if (ind.r == -1)
			return (close_fds(tmp->pipefd, other->open1), ERROR);
		ind.r = dup2(tmp->pipefd[WRITE], 1);
		if (ind.r == -1)
			return (close_fds(tmp->pipefd, other->open1), ERROR);
		close_fds(tmp->pipefd, other->open1);
		return (SUCCESSFUL);
	}
	ind.r = dupping2(tmp, other, position);
	return (SUCCESSFUL);
}

int	check_file(t_cmd *tmp, t_cmd *cmd, t_other *other, int flag)
{
	if (tmp->red)
	{
		while (tmp->red)
		{
			if (tmp->red->red_type == HERDOOC)
			{
				tmp->is_limiter = 1;
				tmp->limiter = cmd->red->file;
				pipping(tmp, cmd, other, 2);
				tmp->open1 = tmp->pipedoc[READ];
				if (tmp->next)
					tmp->open2 = tmp->pipefd[WRITE];
			}
			else if (tmp->red->red_type == (APPEND || REDIR_OUT))
			{
				if (flag != 0)
					tmp->open1 = tmp->pipefd[READ];
				if (tmp->red->red_type == REDIR_OUT)
					tmp->open2 = open (tmp->red->file, O_CREAT | O_WRONLY | O_TRUNC);
				else if (tmp->red->red_type == APPEND)
					tmp->open2 = open (tmp->red->file, O_CREAT | O_WRONLY | O_APPEND);
			}
			else if (tmp->red->red_type == REDIR_IN)
			{
				tmp->open1 = open (tmp->red->file, O_RDONLY | O_TRUNC);
				if (tmp->next)
					tmp->open2 = tmp->pipefd[WRITE];
			}
			tmp->red = tmp->red->next;
		}
	}
	else
	{
		if (flag != 0)
			tmp->open1 = tmp->pipefd[READ];
		if (tmp->next)
			tmp->open2 = tmp->pipefd[WRITE];
	}
	return (SUCCESSFUL);
}

int	child_process(t_cmd *tmp, t_cmd *cmd, t_other *other, int position)
{
	t_ind	ind;

	ind.r = 0;
	check_file(tmp, cmd, other, position);
	ind.r = dupping(tmp, other, position);
	if (ind.r == -1)
	{
		if (position == 0)
		{
			if (access("/tmp/here_doc", F_OK) == 0)
				unlink("/tmp/here_doc");
		}
		free_all(&cmd, other);
		return (perror("Error dup2: "), exit(1), 1);
	}
	exec(tmp, cmd, other);
	exit(1);
}

int is_pipe(t_cmd *cmd, t_other	*other)
{
	int	i;

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

int	is_limiter(char *line, char *limiter)
{
	t_ind	ind;

	ind.i = 0;
	while (limiter[ind.i] && limiter[ind.i] == line[ind.i])
		ind.i ++;
	if (limiter[ind.i] == 0 && line[ind.i] == '\n')
		return (TRUE);
	return (FALSE);
}

void heredoc_outp(int a_pipe)
{
	if (a_pipe)
		myputstr("pipe heredoc> ", 1);
	else
		myputstr("heredoc> ", 1);
	return ;
}

int	make_heredoc(t_cmd *cmd, t_other *other)
{
	char	*line;
	t_ind	ind;

	ind.c = 0;
	while (1)
	{
		heredoc_outp(other->a_pipe);
		line = get_next_line(0);
		if (line == NULL && ind.c == 0)
		{
			// close(other->open1);
			return (why_exit("gnl failed\n", 2), 1);
		}
		if (line == NULL && ind.c == 1)
			break ;
		ind.c = 1;
		if (is_limiter(line, cmd->limiter) == TRUE)
			return (free(line), SUCCESSFUL);
		write (cmd->open1, line, mystrlen(line));
		free(line);
	}
	if (line)
		free(line);
	return (close(cmd->open1), cmd->open1 = -1, SUCCESSFUL);
}

int is_heredoc(t_cmd *cmd)
{
	if (cmd->red)
	{
		if (cmd->red->red_type == HERDOOC)
		{
			cmd->red->is_limiter = 1;
			return (1);
		}
	}
	return (0);
}

int	pipping(t_cmd *tmp, t_cmd *cmd, t_other *other, int type)
{
	t_ind	ind;

	if (type == 1)
		ind.r = pipe(tmp->pipefd);
	else
		ind.r = pipe(tmp->red->pipedoc);
	if (ind.r == -1)
	{
		free_all(&cmd, other);
		why_exit("Error: pipe failed\n", FAILED);
	}
	return (SUCCESSFUL);
}

int	set_reds(t_cmd *tmp)
{
	while (tmp->red)
	{
		tmp->red->is_limiter = 0;
		tmp->red->limiter = NULL;
		tmp->red->pipedoc[WRITE] = -3;
		tmp->red->pipedoc[READ] = -3;
	}
}

int	set_fds(t_cmd *tmp)
{
	tmp->open1 = -3;
	tmp->open2 = -3;
	tmp->pipefd[WRITE] = -3;
	tmp->pipefd[READ] = -3;
}

int	execution2(t_cmd *tmp, t_cmd *cmd, t_other *other, int i)
{
	if (tmp->pid == -1)
	{
		free_all(&cmd, other);
		why_exit("Error: fork failed\n", FAILED);
	}
	if (tmp->pid == 0)
		child_process(tmp, cmd, other, i);
	if (i != 0)
		close(other->prev_read);
	if (tmp->next)
	{
		close(tmp->pipefd[WRITE]);
		other->prev_read = tmp->pipefd[READ];
	}
	return (SUCCESSFUL);
}

int  work(t_cmd *cmd, t_env *env, t_other *other)
{
	t_cmd	*tmp;
	t_ind	ind;

	ind.i = 1;
	tmp = cmd;
	while (tmp && other->a_pipe)
	{
		set_fds(tmp);
		if (tmp->next)
			pipping(tmp, cmd, other, 1);
		while (tmp->red)
		{
			set_reds(tmp);
			if (is_heredoc(tmp))
				make_heredoc(tmp, other);
		}
		ind.i++;
		tmp = tmp->next;
	}
	tmp = cmd;
	ind.i = 0;
	while (tmp)
	{
		tmp->pid = fork();
		execution2(tmp, cmd, other, ind.i);
		ind.i++;
		tmp = tmp->next;
	}
	ind.t = 0;
	while (ind.t < ind.i)
	{
		wait(NULL);
		ind.t ++;
	}
	return (SUCCESSFUL);
}

int exec (t_cmd *cmd, t_env *env)
{
	t_other	other;
	// t_env *tmp;
	// tmp = env;

	// test(cmd);
	is_pipe(cmd, &other);
	work(cmd, env, &other);
	return (0);
}