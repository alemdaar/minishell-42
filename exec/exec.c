/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelhasso <oelhasso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 17:26:38 by oelhasso          #+#    #+#             */
/*   Updated: 2025/06/27 20:15:05 by oelhasso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "header.h"
// #include "gnl/get_next_line.h"

// void test(t_cmd *cmd, t_env **env)
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
// 	while (env)
// 	{
// 		printf ("key : %s\n", env->key);
// 		printf ("value %s\n", env->value);
// 		env = env->next;
// 	}
// }

int	exec(t_cmd *tmp, t_cmd *cmd, t_other *other)
{
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

int	dupping(t_cmd *tmp, t_other *other)
{
	t_ind	ind;

	ind.r = dup2(tmp->open1, 0);
	if (ind.r == -1)
	{
		// close_fds();
		return (ERROR);
	}
	ind.r = dup2(tmp->open2, 1);
	if (ind.r == -1)
	{
		// close_fds();
		return (ERROR);
	}
	// close_fds();
	return (SUCCESSFUL);
}

int	check_file(t_cmd *tmp, t_cmd *cmd, t_other *other, int flag)
{
	if (other->a_pipe)
	{
		if (flag == 0)
		{
			tmp->next->prev_pipe = tmp->pipefd[READ];
			tmp->open2 = tmp->pipefd[WRITE];
		}
		else if (tmp->next == NULL)
			tmp->open1 = tmp->prev_pipe;
		else
		{
			tmp->open1 = tmp->prev_pipe;
			tmp->open2 = tmp->pipefd[WRITE];
			tmp->next->prev_pipe = tmp->pipefd[READ];
		}
	}
	if (tmp->red)
	{
		while (tmp->red)
		{
			if (tmp->red->red_type == HERDOOC)
			{
				tmp->red->limiter = cmd->red->file;
				tmp->open1 = tmp->red->pipedoc[READ];
			}
			else if (tmp->red->red_type == REDIR_OUT)
			{
				tmp->open2 = open (tmp->red->file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
				if (tmp->open2 == -1)
					return (file_failed(tmp->red->file), 1);
			}
			else if (tmp->red->red_type == APPEND)
			{
				tmp->open2 = open (tmp->red->file, O_CREAT | O_WRONLY | O_APPEND, 0644);
				if (tmp->open2 == -1)
					return (file_failed(tmp->red->file), 1);
			}
			else if (tmp->red->red_type == REDIR_IN)
			{
				tmp->open1 = open (tmp->red->file, O_RDONLY);
				if (tmp->open1 == -1)
					return (file_failed(tmp->red->file), 1);
			}
			tmp->red = tmp->red->next;
		}
	}
	int i = 1;
	int r;
	while (tmp->commands[i])
	{
		r = open (tmp->commands[i], O_RDONLY);
		if (r == -1)
			return (file_failed(tmp->commands[i]), 1);
		i++;
	}
	return (SUCCESSFUL);
}

int is_builtin()
{
	// echo with option -n
	// cd with only a relative or absolute path
	// pwd with no options
	// export with no options
	// unset with no options
	// env with no options or arguments
	// exit with no options
	
}

int check_cmd(t_cmd *tmp, t_other *other)
{
	if (is_builtin(tmp, other))
		handle_builtin();
	else
	{
		
	}
}

int	child_process(t_cmd *tmp, t_cmd *cmd, t_other *other, int position)
{
	t_ind	ind;

	ind.r = 0;
	check_file(tmp, cmd, other, position);
	ind.r = dupping(tmp, other);
	if (ind.r == -1)
	{
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

int	make_heredoc(t_cmd *tmp, t_cmd *cmd, t_other *other)
{
	char	*line;
	t_ind	ind;

	ind.c = 0;
	pipping(tmp, cmd, other, 2);
	while (1)
	{
		myputstr("> ", 1);
		line = get_next_line(0);
		if (line == NULL && ind.c == 0)
		{
			// close(other->open1);
			return (why_exit("gnl failed\n", 2), 1);
		}
		if (line == NULL && ind.c == 1)
			break ;
		ind.c = 1;
		if (is_limiter(line, tmp->red->limiter) == TRUE)
			return (free(line), SUCCESSFUL);
		write (tmp->red->pipedoc[WRITE], line, mystrlen(line));
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
			return (1);
		}
	}
	return (0);
}

int	set_reds(t_cmd *tmp)
{
	tmp->red->limiter = NULL;
	tmp->red->pipedoc[WRITE] = -3;
	tmp->red->pipedoc[READ] = -3;
}

int	set_fds(t_cmd *tmp)
{
	tmp->open1 = -3;
	tmp->open2 = -3;
	tmp->pipefd[WRITE] = -3;
	tmp->pipefd[READ] = -3;
	tmp->is_limiter = 0;
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
	while (tmp)
	{
		set_fds(tmp);
		ind.c = check_cmd(tmp, other);
		if (ind.c == ERROR)
			return (free_all(cmd, other), exit(1), 1);
		ind.r = fill_argument(&tmp);
		if (ind.r == ERROR)
			return (free_all(cmd, other), exit(1), 1);
		if (tmp->next)
			pipping(tmp, cmd, other, 1);
		while (tmp->red)
		{
			set_reds(tmp);
			if (is_heredoc(tmp))
				make_heredoc(tmp, cmd, other);
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

int	find_path(t_other *other, t_env **env)
{
	int		i;
	t_env	*tmp;

	i = 0;
	tmp = *env;
	while (tmp)
	{
		if (tmp->key[0] == 'P' && tmp->key[1] == 'A')
		{
			if (tmp->key[2] == 'T' && tmp->key[3] == 'H')
			{
				if (tmp->key[4] == '=')
				{
					other->all_path = tmp->value;
					return (SUCCESSFUL);
				}
			}
		}
		tmp = tmp->next;
	}
	other->all_path = NULL;
	return (SUCCESSFUL);
}

void	count_path(t_other *other)
{
	t_ind	ind;

	ind.i = 0;
	other->count_path = 0;
	while (other->all_path[ind.i])
	{
		while (other->all_path[ind.i] && other->all_path[ind.i] != ':')
			ind.i ++;
		if (other->all_path[ind.i] == ':')
		{
			other->count_path ++;
			ind.i ++;
		}
		if (!other->all_path[ind.i])
			other->count_path ++;
	}
	return ;
}

void	fill_path(t_other *other, t_ind *ind)
{
	int	a;

	a = 0;
	other->paths[ind->c] = malloc (sizeof(char) * (ind->j - ind->i) + 1);
	if (!other->paths[ind->c])
		free_all(NULL, other);
	while (ind->i < ind->j)
		other->paths[ind->c][a++] = other->all_path[ind->i++];
	other->paths[ind->c][a] = 0;
	return ;
}

void	edit_paths(t_other *other, t_env **env)
{
	t_ind	ind;

	find_path(other, env);
	count_path(other);
	if (other->all_path == NULL)
		return ;
	other->paths = malloc (sizeof(char *) * other->count_path);
	if (!other->paths)
		why_exit("other->paths allocation failed !", FAILED);
	ind.i = 0;
	ind.c = 0;
	ind.j = ind.i;
	while (other->all_path[ind.i])
	{
		while (other->all_path[ind.j] && other->all_path[ind.j] != ':')
			ind.j++;
		if (!other->all_path[ind.j] || other->all_path[ind.j] == ':')
			fill_path(other, &ind);
		if (other->all_path[ind.j] == ':')
			ind.j ++;
		ind.i = ind.j;
		ind.c ++;
	}
	return ;
}

int execution(t_cmd *cmd, t_env **env)
{
	t_other	other;
	// t_env *tmp;
	// tmp = env;

	// test(cmd);
	is_pipe(cmd, &other);
	edit_paths(&other, env);
	work(cmd, env, &other);
	return (0);
}
