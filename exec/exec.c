/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelhasso <oelhasso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 17:26:38 by oelhasso          #+#    #+#             */
/*   Updated: 2025/06/29 22:23:55 by oelhasso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "header.h"
#include "gnl/get_next_line.h"

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
// fd 1
void why_exit(char *str, int flag)
{
	printf ("%s", str);
	exit(flag);
}

size_t mystrlen(char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

void nothing(void *tmp)
{
	if (tmp)
		return ;
	return ;
}

int	exec(t_cmd *tmp, t_other *other, t_env *env)
{
	nothing(other);
	if (tmp->path_cmd == NULL)
	{
		// free_all(other);
		printf ("Error: %s command not found\n", tmp->commands[0]);
		return (FAILED);
	}
	if (execve(tmp->path_cmd, tmp->argument, NULL) == ERROR)
	{
		if (access("/tmp/here_doc", F_OK) == 0)
			unlink("/tmp/here_doc");
		// free_all(other);
		perror ("execve: ");
		exit(1);
	}
	return (SUCCESSFUL);
}

int	dupping(t_cmd *tmp, t_other *other)
{
	t_ind	ind;

	ind.r = -99;
	nothing(other);
	if (tmp->open1 != -3)
		ind.r = dup2(tmp->open1, 0);
	#ifdef DEBUG
	printf ("fd 1 : %d\n", tmp->open1);
	#endif
	if (ind.r == -1)
	{
		printf (".............1\n");
		// printf (".............1\n");
		// close_fds();
		return (ERROR);
	}
	#ifdef DEBUG
	printf ("fd 2;) : %d\n", tmp->open2);
	#endif
	if (tmp->open2 != -3)
		ind.r = dup2(tmp->open2, 1);
	if (ind.r == -1)
	{
		// close_fds();
		return (ERROR);
	}
	// close_fds();
	return (SUCCESSFUL);
}

int	check_file(t_cmd *tmp, t_other *other, int flag)
{
	if (other->a_pipe)
	{
		if (flag == 0)
		{
			tmp->open2 = tmp->pipefd[WRITE];
		}
		else if (tmp->next == NULL)
		{
			tmp->open1 = tmp->prev->pipefd[READ];
		}
		else
		{
			tmp->open1 = tmp->prev->pipefd[READ];
			tmp->open2 = tmp->pipefd[WRITE];
		}
	}
	if (tmp->red)
	{
		while (tmp->red)
		{
			if (tmp->red->red_type == HERDOOC)
			{
				tmp->red->limiter = tmp->red->file;
				tmp->open1 = tmp->red->pipedoc[READ];
			}
			else if (tmp->red->red_type == REDIR_OUT)
			{
				tmp->open2 = open (tmp->red->file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
				if (tmp->open2 == -1)
					return (1); // return (file_failed(tmp->red->file), 1);
			}
			else if (tmp->red->red_type == APPEND)
			{
				tmp->open2 = open (tmp->red->file, O_CREAT | O_WRONLY | O_APPEND, 0644);
				if (tmp->open2 == -1)
					return (1); // return (file_failed(tmp->red->file), 1);
			}
			else if (tmp->red->red_type == REDIR_IN)
			{
				tmp->open1 = open (tmp->red->file, O_RDONLY);
				if (tmp->open1 == -1)
					return (1); // return (file_failed(tmp->red->file), 1);
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
			return (1); // return (file_failed(tmp->commands[i]), 1);
		i++;
	}
	return (SUCCESSFUL);
}

char	*mixem(t_cmd *cmd, t_other *other, int path_ind)
{
	char	*str;
	int		i;
	int		j;
	int		len;

	if (other->all_path == NULL || path_ind == -1)
		str = malloc (mystrlen(cmd->commands[0]) + 1);
	else
	{
		printf ("ind : %d\n", path_ind);
		printf ("path : %s\n", other->paths[path_ind]);
		len = mystrlen(other->paths[path_ind]) + mystrlen(cmd->commands[0]);
		str = malloc (len + 2);
	}
	i = 0;
	j = 0;
	if (path_ind != -1)
	{
		while (other->paths[path_ind][j])
			str[i++] = other->paths[path_ind][j++];
		j = 0;
		str[i++] = '/';
	}
	while (cmd->commands[0][j])
		str[i++] = cmd->commands[0][j++];
	str[i] = 0;
	return (str);
}

int	check_access(t_cmd *cmd, t_other *other, int path_ind)
{
	cmd->path_cmd = mixem(cmd, other, path_ind);
	if (!cmd->path_cmd)
		return (printf("full path failed allocation\n"), ERROR);
	printf ("cmd : %s\n", cmd->path_cmd);
	if (access(cmd->path_cmd, F_OK) == SUCCESSFUL)
	{
		if (access(cmd->path_cmd, X_OK) == SUCCESSFUL)
			return (TRUE);
	}
	return (free(cmd->path_cmd), cmd->path_cmd = NULL, FALSE);
}

int is_builtin(t_cmd *tmp, t_other *other)
{
	// ◦ echo with option -n
	// ◦ cd with only a relative or absolute path
	// ◦ pwd with no options
	// ◦ export with no options
	// ◦ unset with no options
	// ◦ env with no options or arguments
	// ◦ exit with no options
	// if (is_equal(tmp->commands[0]), "echo")
	// 	return (SUCCESSFUL);
	// if (is_equal(tmp->commands[0]), "cd")
	// 	return (SUCCESSFUL);
	// if (is_equal(tmp->commands[0]), "pwd")
	// 	return (SUCCESSFUL);
	// if (is_equal(tmp->commands[0]), "export")
	// 	return (SUCCESSFUL);
	// if (is_equal(tmp->commands[0]), "unset")
	// 	return (SUCCESSFUL);
	// if (is_equal(tmp->commands[0]), "env")
	// 	return (SUCCESSFUL);
	// if (is_equal(tmp->commands[0]), "exit")
	// 	return (SUCCESSFUL);
	if (tmp)
		return (FAILED);
	if (other)
		return (FAILED);
	return (FAILED);
}

int	check_cmd(t_cmd *cmd, t_other *other)
{
	t_ind	ind;

	ind.i = 0;
	ind.c = FALSE;
	if (is_builtin(cmd, other) == SUCCESSFUL)
	{
		printf ("builtin\n");
		return (SUCCESSFUL);
	}
	while (ind.i < other->count_path)
	{
		ind.c = check_access(cmd, other, ind.i);
		if (ind.c == TRUE)
			break ;
		else if (ind.c == ERROR)
			return (ERROR);
		ind.i ++;
	}
	if (ind.c == FALSE)
	{
		ind.c = check_access(cmd, other, -1);
		if (ind.c == FALSE)
			return (cmd->path_cmd = NULL, FAILED);
	}
	return (SUCCESSFUL);
}

int	fill_argument(t_cmd *tmp)
{
	t_ind	ind;

	ind.i = 0;
	ind.f = 0;
	tmp->argument = malloc (sizeof(char *) * (tmp->ar + 1));
	if (!tmp->argument)
		return (printf("argument failed !\n"), ERROR); //free_all(other),
	tmp->argument[ind.i] = malloc (mystrlen(tmp->commands[ind.i]) + 1);
	if (!tmp->argument[ind.i])
		return (printf("tmp->argument[ind.i] allocation failed\n"), ERROR);
	ind.t = 0;
	while (tmp->commands[ind.i][ind.f])
		tmp->argument[ind.i][ind.t++] = tmp->commands[ind.i][ind.f++];
	ind.i ++;
	if (tmp->commands[ind.i] == NULL)
		return (tmp->argument[ind.i] = NULL, SUCCESSFUL);
	while (ind.i < tmp->ar)
	{
		tmp->argument[ind.i] = tmp->commands[ind.i];
		ind.i ++;
	}
	tmp->argument[ind.i] = NULL;
	return (SUCCESSFUL);
}

int	child_process(t_cmd *tmp, t_other *other, int position)
{
	t_ind	ind;

	ind.r = 0;
	check_file(tmp, other, position);
	ind.r = dupping(tmp, other);
	if (ind.r == -1)
	{
		// free_all(other);
		return (perror("Error dup2: "), exit(1), 1);
	}
	ind.r = exec(tmp, other);
	return (ind.r);
}

int is_pipe(t_cmd *cmd, t_other	*other)
{
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
		if (cmd || other)
			why_exit("Error: pipe failed\n", FAILED);
		// free_all(other);
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
		printf("> ");
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
	return (SUCCESSFUL);
}

int	set_up(t_cmd *tmp)
{
	tmp->open1 = -3;
	tmp->open2 = -3;
	tmp->pipefd[WRITE] = -3;
	tmp->pipefd[READ] = -3;
	tmp->is_limiter = 0;
	tmp->ar = 1;
	return (SUCCESSFUL);
}

int	execution2(t_cmd *tmp, t_other *other, t_env *env, int i)
{
	t_ind ind;

	nothing(env);
	if (tmp->pid == -1)
	{
		// free_all(other);
		why_exit("Error: fork failed\n", FAILED);
	}
	if (tmp->pid == 0)
	{
		ind.r = child_process(tmp, other, i);
		exit (ind.r);
	}
	if (i != 0)
		close(other->prev_read);
	if (tmp->next)
	{
		close(tmp->pipefd[WRITE]);
		other->prev_read = tmp->pipefd[READ];
	}
	return (SUCCESSFUL);
}

void count_args(t_cmd *tmp)
{
	int i = 0;
	while (tmp->commands[i])
		i++;
	tmp->ar = i;
	return ;
}
int  work(t_cmd *cmd, t_env *env, t_other *other)
{
	t_cmd	*tmp;
	t_ind	ind;

	ind.i = 1;
	tmp = cmd;
	while (tmp)
	{
		set_up(tmp);
		ind.c = check_cmd(tmp, other);
		if (ind.c == ERROR)
			return (exit(1), 1); // free_all(other), 
		count_args(tmp);
		ind.r = fill_argument(tmp);
		if (ind.r == ERROR)
			return (exit(1), 1); //free_all(other), 
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
		execution2(tmp, other, env, ind.i);
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

int	find_path(t_other *other, t_env *env)
{
	int		i;
	t_env	*tmp;

	i = 0;
	tmp = env;
	while (tmp)
	{
		if (tmp->key[0] == 'P' && tmp->key[1] == 'A')
		{
			if (tmp->key[2] == 'T' && tmp->key[3] == 'H')
			{
				other->all_path = tmp->value;
				return (SUCCESSFUL);
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
	if (other->all_path == NULL)
		return ;
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
	{
		// free_all(NULL, other);
		printf ("malloc failed\n");
		exit(1);
	}
	while (ind->i < ind->j)
		other->paths[ind->c][a++] = other->all_path[ind->i++];
	other->paths[ind->c][a] = 0;
	return ;
}

void	edit_paths(t_other *other, t_env *env)
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

int execution(t_cmd *cmd, t_env *env)
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
