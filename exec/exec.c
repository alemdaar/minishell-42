/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelhasso <oelhasso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 17:26:38 by oelhasso          #+#    #+#             */
/*   Updated: 2025/07/07 23:49:20 by oelhasso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "header.h"
#include "gnl/get_next_line.h"

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
void nothing(void *tmp);

// void close_reds(t_cmd *tmp, int flag)
// {
// 	t_red *copy;

// 	if (flag == -65656)
// 		return;
// 	copy = tmp->red;
// 	while (copy)
// 	{
// 		if (copy)
// 		{
// 			if (copy->pipedoc[READ] != tmp->open1 && copy->pipedoc[READ] != tmp->open2)
// 			{
// 				close (copy->pipedoc[READ]);
// 				copy->pipedoc[READ] = -3;
// 			}
// 			if (copy->pipedoc[WRITE])
// 			{
// 				close (copy->pipedoc[WRITE]);
// 				copy->pipedoc[WRITE] = -3;
// 			}
// 		}
// 		else
// 		{
// 			if (copy->pipedoc[READ])
// 			{
// 				close (copy->pipedoc[READ]);
// 				copy->pipedoc[READ] = -3;
// 			}
// 			if (copy->pipedoc[WRITE])
// 			{
// 				close (copy->pipedoc[WRITE]);
// 				copy->pipedoc[WRITE] = -3;
// 			}
// 		}
// 		copy = copy->next;
// 	}
// }

void close_fds(t_cmd *tmp, t_other *other)
{
	t_red	*red_tmp;

	nothing(other);
	if (tmp->open1 != -3)
	{
		dprintf (other->debug, "+closed in %s -> tmp->open1 : %d\n", tmp->commands[0] ,tmp->open1);
		close(tmp->open1);
		tmp->open1 = -3;
	}
	if (tmp->open2 != -3)
	{
		dprintf (other->debug, "+closed in %s -> tmp->open2 : %d\n", tmp->commands[0], tmp->open2);
		close(tmp->open2);
		tmp->open2 = -3;
	}
	if (tmp->pipefd[READ] != -3)
	{
		dprintf (other->debug, "+closed in %s -> tmp->pipefd[READ] : %d\n", tmp->commands[0], tmp->pipefd[READ]);
		close(tmp->pipefd[READ]);
		tmp->pipefd[READ] = -3;
	}
	if (tmp->pipefd[WRITE] != -3)
	{
		dprintf (other->debug, "+closed in %s -> tmp->pipefd[WRITE] : %d\n", tmp->commands[0], tmp->pipefd[WRITE]);
		close(tmp->pipefd[WRITE]);
		tmp->pipefd[WRITE] = -3;
	}
	red_tmp = tmp->red;
	while (red_tmp)
	{
		if (tmp->red->pipedoc[READ] != -3)
		{
			close(tmp->red->pipedoc[READ]);
			tmp->red->pipedoc[READ] = -3;
		}
		if (tmp->red->pipedoc[WRITE] != -3)
		{
			close(tmp->red->pipedoc[WRITE]);
			tmp->red->pipedoc[WRITE] = -3;
		}
		red_tmp = red_tmp->next;
	}
	return ;
}

void close_all_fds(t_cmd *cmd, t_other *other)
{
	nothing(other);
	while (cmd)
	{
		if (cmd->open1 != -3)
		{
			dprintf (other->debug, "closed in %s -> cmd->open1 : %d\n", cmd->commands[0], cmd->open1);
			close(cmd->open1);
			cmd->open1 = -3;
		}
		if (cmd->open2 != -3)
		{
			dprintf (other->debug, "closed in %s -> cmd->open2 : %d\n", cmd->commands[0], cmd->open2);
			close(cmd->open2);
			cmd->open2 = -3;
		}
		if (cmd->pipefd[READ] != -3)
		{
			dprintf (other->debug, "closed in %s -> cmd->pipefd[READ] : %d\n", cmd->commands[0], cmd->pipefd[READ]);
			close(cmd->pipefd[READ]);
			cmd->pipefd[READ] = -3;
		}
		if (cmd->pipefd[WRITE] != -3)
		{
			dprintf (other->debug, "closed in %s -> cmd->pipefd[WRITE] : %d\n", cmd->commands[0], cmd->pipefd[WRITE]);
			close(cmd->pipefd[WRITE]);
			cmd->pipefd[WRITE] = -3;
		}
		while (cmd->red)
		{
			if (cmd->red->pipedoc[READ] != -3)
			{
				close(cmd->red->pipedoc[READ]);
				cmd->red->pipedoc[READ] = -3;
			}
			if (cmd->red->pipedoc[WRITE] != -3)
			{
				close(cmd->red->pipedoc[WRITE]);
				cmd->red->pipedoc[WRITE] = -3;
			}
			cmd->red = cmd->red->next;
		}
		cmd = cmd->next;
	}
}

int free_all(t_other *other)
{
	int	i;

	i = 0;
	while (i < other->count_path && other->paths)
	{
		if (other->paths[i] == NULL)
			return (free(other->paths), other->paths = NULL, FAILED);
		free(other->paths[i]);
		other->paths = NULL;
		i++;
	}
	if (other->paths == NULL)
		return (FAILED);
	else
	{
		free(other->paths);
		other->paths = NULL;
	}
	while (other->orig_cmd)
	{
		if (other->orig_cmd->path_cmd)
		{
			free(other->orig_cmd->path_cmd);
			other->orig_cmd->path_cmd = NULL;
		}
		i = 0;
		while (other->orig_cmd->argument[i])
		{
			free(other->orig_cmd->argument[i]);
			other->orig_cmd->argument[i] = NULL;
			i ++;
		}
		if (other->orig_cmd->argument)
		{
			free(other->orig_cmd->argument);
			other->orig_cmd->argument = NULL;
		}
		other->orig_cmd = other->orig_cmd->next;
	}
	if (other->envr)
	{
		free(other->envr);
		other->envr = NULL;
	}
	return (0);
	// parcing free;
}
void why_exit(char *str, int flag)
{
	if (flag == SUCCESSFUL)
		perror (str);
	else if (flag == FAILED)
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

int	is_equal(char *command, char *b_in)
{
	int i = 0;
	while (command[i] && b_in[i] && b_in[i] == command[i])
		i++;
	if (b_in[i] == command[i])
		return (1);
	return (0);
}

int echo_nl(char *opt)
{
	int i = 0;
	if (opt[i++] != '-')
		return (1);
	if (!opt[i])
		return (1);
	while (opt[i])
	{
		if (opt[i] != '-')
			return (1);
		i++;
	}
	return (0);
}

int builtin_echo(t_cmd *tmp)
{
    int i = 1;
    int newline = 1;

    if (tmp->commands[1])
    {
		if (tmp->commands[1][0])
		{
			if (echo_nl(tmp->commands[1]) == 1)
        		newline = 0;
		}
    }
    {
    while (tmp->commands[i])
		printf ("%s", tmp->commands[i++]);
    if (newline)
        printf("\n");
    return (0);
}

int run_bin(t_cmd *tmp, t_other *other)
{
	if (is_equal(tmp->commands[0], "echo"))
	{
		builtin_echo();
		return (SUCCESSFUL);
	}
	if (is_equal(tmp->commands[0], "cd"))
	{
		if (chdir(args[1]) != 0)
    	{
    	    perror("cd");
    	    return 1;
    	}
		return (SUCCESSFUL);
	}
	if (is_equal(tmp->commands[0], "pwd") == 1)
		return (SUCCESSFUL);
	if (is_equal(tmp->commands[0], "export"))
		return (SUCCESSFUL);
	if (is_equal(tmp->commands[0], "unset"))
		return (SUCCESSFUL);
	if (is_equal(tmp->commands[0], "env"))
		return (SUCCESSFUL);
	if (is_equal(tmp->commands[0], "exit"))
		return (SUCCESSFUL);
	return (FAILED);
}

int	exec(t_cmd *tmp, t_other *other)
{
	int	r;

	if (tmp->bin == 1)
	{
		r = run_bin(tmp, other);
		return (r);
	}
	else if (tmp->path_cmd == NULL)
	{
		free_all(other);
		printf ("Error: %s command not found\n", tmp->commands[0]);
		return (FAILED);
	}
	int i = 0;
	while (tmp->argument[i])
		dprintf (other->debug, "arg ==== : %s\n", tmp->argument[i++]);
	dprintf (other->debug, "EXECVE TIME !\n");
	dprintf (other->debug, "HAVE A LOOK ON THE ENV : %s\n", other->envr[0]);
	dprintf (other->debug, "TEST=======\n");
	if (other->envr == NULL)
		dprintf (other->debug, "ENV IS NULL\n");
	dprintf (other->debug, "TEST\n");
	if (execve(tmp->path_cmd, tmp->argument, other->envr) == ERROR)
	{
		if (access("/tmp/here_doc", F_OK) == 0)
			unlink("/tmp/here_doc");
		free_all(other);
		perror ("execve: ");
		exit(1);
	}
	dprintf (other->debug, "EXECVE ENDED !\n");
	return (SUCCESSFUL);
}

int	dupping(t_cmd *tmp, t_other *other)
{
	t_ind	ind;

	ind.r = -99;
	nothing(other);
	dprintf (other->debug, "fd in \"%s\" command, open 1 is : %d\n", tmp->commands[0], tmp->open1);
	dprintf (other->debug, "fd in \"%s\" command, open 2 is : %d\n", tmp->commands[0], tmp->open2);
	if (tmp->open1 != -3 && tmp->open1 != -1)
	{
		ind.r = dup2(tmp->open1, 0);
		if (ind.r == -1)
		{
			printf ("error in \"%s\" command, open 1", tmp->commands[0]);
			return (ERROR);
		}
		dprintf (other->debug, "closed in %s -> tmp->open1 : %d\n", tmp->commands[0], tmp->open1);
		close(tmp->open1);
		tmp->open1 = -3;
	}
	if (tmp->open2 != -3 && tmp->open2 != -1)
	{
		ind.r = dup2(tmp->open2, 1);
		if (ind.r == -1)
		{
			printf ("error in \"%s\" command, open 2", tmp->commands[0]);
			return (ERROR);
		}
		dprintf (other->debug, "closed in %s -> tmp->open2 : %d\n", tmp->commands[0], tmp->open2);
		close(tmp->open2);
		tmp->open2 = -3;
	}
	dprintf (other->debug, "close_fds in dup, cmd : %s\n", tmp->commands[0]);
	close_fds(tmp, other);
	return (SUCCESSFUL);
}

int	check_file(t_cmd *tmp, t_other *other, int flag)
{
	t_red	*copy_red;

	if (other->a_pipe)
	{
		if (flag == 0)
		{
			tmp->open2 = tmp->pipefd[WRITE];
			// dprintf (other->debug, "check read : %d, and its in cmd : %s\n", tmp->pipefd[READ], tmp->commands[0]);
			close(tmp->pipefd[READ]);
		}
		else if (tmp->next == NULL)
		{
			tmp->open1 = tmp->prev->pipefd[READ];
			// dprintf (other->debug, "check open : %d, and its in cmd : %s\n", tmp->open1, tmp->commands[0]);
			// dprintf (other->debug, "check open OWNER : %d, and its in cmd : %s\n", tmp->open1, tmp->commands[0]);
		}
		else
		{
			tmp->open1 = tmp->prev->pipefd[READ];
			tmp->open2 = tmp->pipefd[WRITE];
			close(tmp->pipefd[READ]);
		}
	}
	copy_red = tmp->red;
	while (copy_red)
	{
		if (copy_red->red_type == HERDOOC)
		{
			// printf ("doc\n");
			// printf ("first open is : %d\n", tmp->open1);
			if (tmp->pipefd[READ] != -3  && other->a_pipe)
			{
				// printf ("closed pipe\n");
				close(tmp->pipefd[READ]);
				tmp->pipefd[READ] = -3;
			}
			if (tmp->open1 != -3)
			{
				// printf ("closed open1\n");
				dprintf (other->debug, "closed in %s -> tmp->pipefd[READ] : %d\n", tmp->commands[0], tmp->pipefd[READ]);
				close(tmp->open1);
				tmp->open1 = -3;
			}
			if (tmp->old_doc->pipedoc[READ] == copy_red->pipedoc[READ])
				tmp->open1 = copy_red->pipedoc[READ];
			// printf ("open is : %d\n", tmp->open1);
			// dprintf (other->debug, "check open1 init is : %d,  cmd : %s\n", copy_red->pipedoc[READ], tmp->commands[0]);
			// dprintf (other->debug, "****see limiter : '%s',  cmd : %s\n", copy_red->limiter, tmp->commands[0]);
			// while (1);
		}
		else if (copy_red->red_type == REDIR_OUT)
		{
			// printf ("first open is : %d\n", tmp->open2);
			if (tmp->pipefd[WRITE] != -3 && other->a_pipe)
			{
				close(tmp->pipefd[WRITE]);
				tmp->pipefd[WRITE] = -3;
			}
			if (tmp->open2 != -3)
			{
				// printf ("closed open2\n");
				close (tmp->open2);
				tmp->open2 = -3;
			}
			// dprintf (other->debug, "REDIR_OUT in address -> %p\n", &tmp->open2);
			tmp->open2 = open (copy_red->file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
			if (tmp->open2 == -1)
			{
				return (perror("Error: "), -1);
			}
			// printf ("open is : %d\n", tmp->open2);
		}
		else if (copy_red->red_type == APPEND)
		{
			if (tmp->pipefd[WRITE] != -3 && other->a_pipe)
			{
				// printf ("closed open2\n");
				close(tmp->pipefd[WRITE]);
				tmp->pipefd[WRITE] = -3;
			}
			// dprintf (other->debug, "APPEND in address -> %p\n", &tmp->open2);
			if (tmp->open2 != -3)
			{
				close (tmp->open2);
				tmp->open2 = -3;
			}
			tmp->open2 = open (copy_red->file, O_CREAT | O_WRONLY | O_APPEND, 0644);
			if (tmp->open2 == -1)
			{
				return (perror("Error: "), -1);
			}
			// printf ("open is : %d\n", tmp->open2);
		}
		else if (copy_red->red_type == REDIR_IN)
		{
			// printf ("red in\n");
			// printf ("first open is : %d\n", tmp->open1);
			if (tmp->pipefd[READ] != -3 && other->a_pipe)
			{
				// printf ("closed pipe\n");
				close(tmp->pipefd[READ]);
				tmp->pipefd[READ] = -3;
			}
			if (tmp->open1 != -3)
			{
				// printf ("closed open1\n");
				// printf ("closed open1\n");
				close (tmp->open1);
				tmp->open1 = -3;
			}
			// dprintf (other->debug, "REDIR_in in address -> %p\n", &tmp->open1);
			tmp->open1 = open (copy_red->file, O_RDONLY);
			// printf ("name : %s\n", copy_red->file);
			// printf ("open is : %d\n", tmp->open1);
			if (tmp->open1 == -1)
			{
				return (perror("Error: "), -1);
			}
			// while (1);
			// printf ("open is : %d\n", tmp->open1);
		}
		copy_red = copy_red->next;
	}
	// printf ("final open1 is : %d\n", tmp->open1);
	// printf ("final open2 is : %d\n", tmp->open2);
	// while (1);
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
		// printf ("ind : %d\n", path_ind);
		// printf ("path : %s\n", other->paths[path_ind]);
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
	// printf ("cmd : %s\n", cmd->path_cmd);
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
	if (is_equal(tmp->commands[0], "pwd") == 1)
		return (SUCCESSFUL);
	// if (is_equal(tmp->commands[0]), "export")
	// 	return (SUCCESSFUL);
	// if (is_equal(tmp->commands[0]), "unset")
	// 	return (SUCCESSFUL);
	// if (is_equal(tmp->commands[0]), "env")
	// 	return (SUCCESSFUL);
	// if (is_equal(tmp->commands[0]), "exit")
	// 	return (SUCCESSFUL);
	// if (tmp)
	// 	return (FAILED);
	// if (other)
	// 	return (FAILED);
	return (FAILED);
}

int	check_cmd(t_cmd *cmd, t_other *other)
{
	t_ind	ind;

	ind.i = 0;
	ind.c = FALSE;
	if (is_builtin(cmd, other) == SUCCESSFUL)
	{
		dprintf (other->debug, "builtin\n");
		cmd->bin = 1;
		return (SUCCESSFUL);
	}
	if (other->all_path == NULL)
		return (FAILED);
	// if (abs_path(cmd->commands[0]))
	// {
		
	// }
	// else
	// {
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
	// }
	return (SUCCESSFUL);
}

int	fill_argument(t_cmd *tmp, t_other *other)
{
	t_ind	ind;

	ind.f = 0;
	tmp->argument = malloc (sizeof(char *) * (tmp->ar + 1));
	if (!tmp->argument)
		return (printf("argument failed !\n"), free_all(other), ERROR);
	ind.t = 0;
	tmp->argument[0] = tmp->commands[0];
	ind.i = 1;
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

// int which_case(char *command, const char *dacase)
// {
// 	int	i;

// 	if (command == NULL)
// 		return (FAILED);
// 	i = 0;
// 	while (command[i] && dacase[i] && command[i] == dacase[i])
// 		i++;
// 	if (command[i] != dacase[i])
// 		return (FAILED);
// 	return (SUCCESSFUL);
// }

int	child_process(t_cmd *tmp, t_other *other, int position)
{
	t_ind	ind;

	ind.r = check_file(tmp, other, position);
	if (ind.r == -1)
	{
		dprintf (other->debug, "close_fds in child prc,  cmd : %s after <check file> failed\n", tmp->commands[0]);
		close_fds(tmp, other);
		free_all(other);
		exit(1);
	}
	// dprintf (other->debug, "open1 is : %d,  cmd : %s\n", tmp->open1, tmp->commands[0]);
	// dprintf (other->debug, "open2 is : %d,  cmd : %s\n", tmp->open2, tmp->commands[0]);
	// printf ("......\n");
	// while (1);
	ind.r = dupping(tmp, other);
	if (ind.r == -1)
	{
		// dprintf (other->debug, "close_fds in child prc,  cmd : %s after dupping failed\n", tmp->commands[0]);
		close_fds(tmp, other);
		free_all(other);
		return (perror("Error dup2: "), 1);
	}
	// while (1);
	// printf ("exec time\n");
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
	// printf("limiter : %s\n", limiter);
	// printf("line : %s\n", line);
	while (limiter[ind.i] && limiter[ind.i] == line[ind.i])
		ind.i ++;
	if (limiter[ind.i] == 0 && line[ind.i] == '\n')
		return (TRUE);
	return (FALSE);
}

int	pipping(t_cmd *tmp, t_red *copy, t_other *other, int type)
{
	t_ind	ind;

	if (type == 1)
	{
		// dprintf (other->debug, "PIPE READ of cmd -> %s in address -> %p\n", tmp->commands[0], &tmp->red->pipedoc[READ]);
		// dprintf (other->debug, "PIPE WRITE of cmd -> %s in address -> %p\n", tmp->commands[0], &tmp->red->pipedoc[WRITE]);
		ind.r = pipe(tmp->pipefd);
	}
	else
	{
		ind.r = pipe(copy->pipedoc);
		// dprintf (other->debug, "HEREDOC READ in address -> %p\n", &copy->pipedoc[READ]);
		// dprintf (other->debug, "HEREDOC WRITE in address -> %p\n", &copy->pipedoc[WRITE]);
	}
	if (ind.r == -1)
	{
		free_all(other);
		why_exit("Error: pipe failed\n", FAILED);
	}
	return (SUCCESSFUL);
}

int	make_heredoc(t_cmd *tmp, t_red *red_copy, t_other *other, int flag)
{
	char	*line;
	t_ind	ind;

	ind.c = 0;
	// dprintf (other->debug, "limiter address =: %p\n", &red_copy->limiter);
	// dprintf (other->debug, "limiter : %s\n", red_copy->limiter);
	// printf ("flag : %d\n", flag);
	if (flag != 0)
	{
		if (tmp->old_doc->pipedoc[READ] != -3)
		{
			close (tmp->old_doc->pipedoc[READ]);
			tmp->old_doc->pipedoc[READ] = -3;
		}
	}
	pipping(tmp, red_copy, other, 2);
	while (1)
	{
		write (1, "> ", 2);
		line = get_next_line(0);
		if (line == NULL && ind.c == 0)
		{
			close(red_copy->pipedoc[READ]);
			red_copy->pipedoc[READ] = -3;
			close(red_copy->pipedoc[WRITE]);
			red_copy->pipedoc[WRITE] = -3;
			return (perror("gnl failed: \n"), 1);
		}
		if (line == NULL && ind.c == 1)
			break ;
		ind.c = 1;
		if (is_limiter(line, red_copy->limiter) == TRUE)
		{
			close(red_copy->pipedoc[WRITE]);
			red_copy->pipedoc[WRITE] = -3;
			tmp->old_doc = red_copy;
			return (free(line), SUCCESSFUL);
		}
		write (red_copy->pipedoc[WRITE], line, mystrlen(line));
		free(line);
	}
	if (line)
		free(line);
	close(red_copy->pipedoc[WRITE]);
	red_copy->pipedoc[WRITE] = -3;
	tmp->old_doc = red_copy;
	return (SUCCESSFUL);
}

int	set_reds(t_red *red)
{
	red->limiter = NULL;
	red->pipedoc[WRITE] = -3;
	red->pipedoc[READ] = -3;
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
	tmp->old_doc = NULL;
	tmp->bin = 0;
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
		// while (1);
		ind.r = child_process(tmp, other, i);
		// exit_status();
		exit (ind.r);
	}
	// dprintf (other->debug, "tmp->open1 1: %d\n", tmp->open1);
	// dprintf (other->debug, "tmp->open2 1: %d\n", tmp->open2);
	if (i == 0 && other->a_pipe)
	{
		// printf ("111\n");
		close(tmp->pipefd[WRITE]);
		tmp->pipefd[WRITE] = -3;
	}
	else if (tmp->next && i != 0)
	{
		// printf ("wst\n");
		if (tmp->pipefd[WRITE])
		{
			close(tmp->pipefd[WRITE]);
			tmp->pipefd[WRITE] = -3;
		}
		if (tmp->prev->pipefd[READ])
		{
			close(tmp->prev->pipefd[READ]);
			tmp->prev->pipefd[READ] = -3;
		}
	}
	else if (tmp->next == NULL && other->a_pipe)
	{
		// printf ("last\n");
		if (tmp->prev->pipefd[READ])
		{
			close(tmp->prev->pipefd[READ]);
			tmp->prev->pipefd[READ] = -3;
		}
	}
	// while (1);
	// while (1);
	if (tmp->old_doc)
	{
		if (tmp->old_doc->pipedoc[READ] != -3)
		{
			close(tmp->old_doc->pipedoc[READ]);
			tmp->old_doc->pipedoc[READ] = -3;
		}
	}
	// while (1);
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

int make_array(t_other *other, int len, t_env *env, int pos)
{
	char	array[len];
	int		i;

	i = 0;
	while (env->key[i])
	{
		array[i] = env->key[i];
		i++;
	}
	array[i++] = ':';
	while (env->value[i])
	{
		array[i] = env->value[i];
		i++;
	}
	array[i] = 0;
	other->envr[pos] = array;
	return (SUCCESSFUL);
}

int count_array(t_env *env)
{
	int	i;

	i = 0;
	while (env)
	{
		i++;
		env = env->next;
	}
	return (i);
}

int  work(t_cmd *cmd, t_other *other)
{
	t_cmd	*tmp;
	t_red	*red_copy;
	t_ind	ind;

	ind.i = 0;
	tmp = cmd;
	while (tmp)
	{
		tmp->free_flag = 0;
		set_up(tmp);
		red_copy = tmp->red;
		ind.j = 0;
		while (red_copy)
		{
			// printf ("loop got %d , file : %s\n", red_copy->red_type, red_copy->file);
			set_reds(red_copy);
			if (red_copy->red_type == HERDOOC)
			{
				// printf ("hada heredoc \n");
				red_copy->limiter = red_copy->file;
				make_heredoc(tmp, red_copy, other, ind.j);
				ind.j ++;
			}
			red_copy = red_copy->next;
		}
		// printf ("SALA \n");
		// dprintf (other->debug, "tmp->open1 ----: %d\n", tmp->open1);
		if (tmp->commands[0])
		{
			ind.c = check_cmd(tmp, other);
			if (ind.c == ERROR)
				return (free_all(other), exit(1), 1); 
			count_args(tmp);
			ind.r = fill_argument(tmp, other);
			if (ind.r == ERROR)
				exit(1);
		}
		else
			return (SUCCESSFUL);
		ind.i++;
		tmp = tmp->next;
	}
	tmp = cmd;
	ind.i = 0;
	// while (1);
	if (cmd->next == NULL)
		child_process(cmd, other, 0);
	else
	{
		while (tmp)
		{
			if (tmp->next)
				pipping(tmp, NULL, other, 1);
			// while (1);
			tmp->pid = fork();
			ind.r = execution2(tmp, other, ind.i);
			if (ind.r == ERROR)
			{
				// exit_status(other);
				break;
			}
			// while (1);
			// dprintf (other->debug, "Loop : %d\n", ind.i);
			ind.i++;
			tmp = tmp->next;
		}
		// while (1);
		// dprintf(other->debug, "ENVIR1 TEST : %s\n", other->envr[0]);
		// dprintf(other->debug, "ENVIR2 TEST : %s\n", other->envr[1]);
		while (ind.i--)
		{
			wait(NULL);
			dprintf (other->debug, "waiting %d\n", ind.i);
		}
	}
	close_all_fds(cmd, other);
	dprintf (other->debug, "finished\n");
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
		free_all(other);
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
	// ENTERED THIS PART

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

void protect_it(t_cmd *cmd, t_other *other)
{
	int	i;

	while (cmd)
	{
		cmd->argument = NULL;
		cmd->path_cmd = NULL;
		cmd = cmd->next;
	}
	other->envr = NULL;
	i = 0;
	other->paths = NULL;
}

int execution(t_cmd *cmd, t_env *env, char **ev)
{
	t_other	other;
	// t_env *tmp;
	// tmp = env;

	// test(cmd);
	other.debug = open ("debug", O_RDWR | O_TRUNC);
	if (other.debug == ERROR)
	{
		printf ("debug file failed\n");
		return 1;
	}
	dprintf(other.debug, "debug fd is : %d\n", other.debug);
	protect_it(cmd, &other);
	dprintf(other.debug, "ENVIR1 : %s\n", ev[0]);
	dprintf(other.debug, "ENVIR2 : %s\n", ev[1]);
	other.envr = ev;
	// dprintf(other.debug, "ENVIR1 TEST : %s\n", other.envr[0]);
	// dprintf(other.debug, "ENVIR2 TEST : %s\n", other.envr[1]);
	is_pipe(cmd, &other);
	edit_paths(&other, env);
	work(cmd, &other);
	return (0);
}
