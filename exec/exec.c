/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelhasso <oelhasso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 17:26:38 by oelhasso          #+#    #+#             */
/*   Updated: 2025/07/09 20:37:32 by oelhasso         ###   ########.fr       */
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


int restore_fds(t_other *other)
{
	dup2(other->stdout_copy, STDOUT_FILENO);
	close(other->stdout_copy);
	other->stdout_copy = -3;
	dup2(other->stdin_copy, STDIN_FILENO);
	close(other->stdin_copy);
	other->stdin_copy = -3;
	return (0);
}

void close_fds(t_cmd *tmp)
{
	if (tmp->open1 != -3)
	{
		close(tmp->open1);
		tmp->open1 = -3;
	}
	if (tmp->open2 != -3)
	{
		close(tmp->open2);
		tmp->open2 = -3;
	}
	if (tmp->pipefd[READ] != -3)
	{
		close(tmp->pipefd[READ]);
		tmp->pipefd[READ] = -3;
	}
	if (tmp->pipefd[WRITE] != -3)
	{
		close(tmp->pipefd[WRITE]);
		tmp->pipefd[WRITE] = -3;
	}
	if (tmp->pipedoc[READ] != -3)
	{
		close(tmp->pipedoc[READ]);
		tmp->pipedoc[READ] = -3;
	}
	if (tmp->pipedoc[WRITE] != -3)
	{
		close(tmp->pipedoc[WRITE]);
		tmp->pipedoc[WRITE] = -3;
	}
	return ;
}

void close_all_fds(t_cmd *cmd)
{
	while (cmd)
	{
		if (cmd->open1 != -3)
		{
			close(cmd->open1);
			cmd->open1 = -3;
		}
		if (cmd->open2 != -3)
		{
			close(cmd->open2);
			cmd->open2 = -3;
		}
		if (cmd->pipefd[READ] != -3)
		{
			close(cmd->pipefd[READ]);
			cmd->pipefd[READ] = -3;
		}
		if (cmd->pipefd[WRITE] != -3)
		{
			close(cmd->pipefd[WRITE]);
			cmd->pipefd[WRITE] = -3;
		}
		if (cmd->pipedoc[READ] != -3)
		{
			close(cmd->pipedoc[READ]);
			cmd->pipedoc[READ] = -3;
		}
		if (cmd->pipedoc[WRITE] != -3)
		{
			close(cmd->pipedoc[WRITE]);
			cmd->pipedoc[WRITE] = -3;
		}
		cmd = cmd->next;
	}
	// printf ("DAZ MN HNAYA\n");
}

void free_env(t_other *other)
{
	t_env *tmp;

	// int debug = open ("debug", O_WRONLY);
	if (other->envrp)
	{
		while (other->envrp)
		{
			if (other->envrp->key)
			{
				// dprintf (debug, "address other->envrp->key : %p\n", other->envrp->key);
				// dprintf (debug, "other->envrp->key : %s\n", other->envrp->key);
				free(other->envrp->key);
				// dprintf (debug, "other->envrp->key has been freed succesfully !\n");
				other->envrp->key = NULL;
			}
			if (other->envrp->value)
			{
				// dprintf (debug, "address other->envrp->value : %p\n", other->envrp->value);
				// dprintf (debug, "other->envrp->value : %s\n", other->envrp->value);
				free(other->envrp->value);
				// dprintf (debug, "other->envrp->value has been freed succecfully !\n");
				other->envrp->value = NULL;
			}
			// dprintf (debug, "address other->envrp : %p\n", other->envrp);
			tmp = other->envrp;
			other->envrp = tmp->next;
			free(tmp);
			tmp = NULL;
			// dprintf (debug, "other->envrp has been freed succecfully !\n");
		}
	}
	return ;
}
int free_all(t_other *other)
{
	int		i;
	t_cmd	*tmp;

	i = 0;
	// int debug = open ("debug", O_WRONLY);
	// dprintf (debug, "tester : %s\n", other->paths[0]);
	while (i < other->count_path && other->paths)
	{
		// printf ("free(other->paths[i]);\n");
		if (other->paths[i] == NULL)
			return (free(other->paths), other->paths = NULL, FAILED);
		// dprintf (debug, "paths[i] : %s\n", other->paths[i]);
		// dprintf (debug, "address paths[i] : %p\n", other->paths[i]);
		free(other->paths[i]);
		// dprintf (debug, "paths[i] has been freed succefully !\n");
		other->paths[i] = NULL;
		i++;
	}
	if (other->paths == NULL)
		return (FAILED);
	else
	{
		// printf ("free(other->paths);\n");
		// dprintf (debug, "address other->paths : %p\n", other->paths);
		free(other->paths);
		// dprintf (debug, "other->paths has been freed succecfully !\n");
		other->paths = NULL;
	}
	while (other->orig_cmd)
	{
		if (other->orig_cmd->path_cmd)
		{
			// printf ("FREE other->orig_cmd->path_cmd\n");
			// dprintf (debug, "address orig_cmd->path_cmd : %p\n", other->orig_cmd->path_cmd);
			// dprintf (debug, "orig_cmd->path_cmd : %s\n", other->orig_cmd->path_cmd);
			free(other->orig_cmd->path_cmd);
			// dprintf (debug, "orig_cmd->path_cmd has been freed succesfully !\n");
			other->orig_cmd->path_cmd = NULL;
		}
		i = 0;
		// while (1);
		while (other->orig_cmd->argument[i])
		{
			// printf ("FREE other->orig_cmd->argument[i]\n");
			// dprintf (debug, "address orig_cmd->argument[i] : %p\n", other->orig_cmd->argument[i]);
			// dprintf (debug, "orig_cmd->argument[i] : %s\n", other->orig_cmd->argument[i]);
			free(other->orig_cmd->argument[i]);
			// dprintf (debug, "orig_cmd->argument[i] has been freed succesfully !\n");
			other->orig_cmd->argument[i] = NULL;
			i ++;
		}
		if (other->orig_cmd->argument)
		{
			// printf ("FREE other->orig_cmd->argument\n");
			// dprintf (debug, "address orig_cmd->argument : %p\n", other->orig_cmd->argument);
			free(other->orig_cmd->argument);
			// dprintf (debug, "orig_cmd->argument has been freed succesfully !\n");
			other->orig_cmd->argument = NULL;
		}
		if (other->orig_cmd->commands)
		{
			// printf ("FREE command\n");
			// dprintf (debug, "address commands : %p\n", other->orig_cmd->commands);
			free(other->orig_cmd->commands);
			// dprintf (debug, "orig_cmd->commands has been freed succesfully !\n");
			other->orig_cmd->path_cmd = NULL;
		}
		// printf ("FREE orig_cmd\n");
		// dprintf (debug, "address orig_cmd : %p\n", other->orig_cmd);
		tmp = other->orig_cmd;
		other->orig_cmd = other->orig_cmd->next;
		free(tmp);
		// dprintf (debug, "orig_cmd has been freed succesfully !\n");
		tmp = NULL;
	}
	// while (1);
	return (0);
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

int echo_nl(t_cmd *tmp, int ind)
{
	while (tmp->commands[ind])
	{
		int i = 0;
		if (tmp->commands[ind][i++] != '-')
			return (ind);
		if (!tmp->commands[ind][i])
			return (ind);
		while (tmp->commands[ind][i])
		{
			if (tmp->commands[ind][i] != 'n')
				return (ind);
			i++;
		}
		ind++;
	}
	return (ind);
}

int builtin_echo(t_cmd *tmp)
{
    int i = 1;
    int j = 1;
    int c = 0;
    int newline = 1;

	if (tmp->commands[i])
    {
		if (tmp->commands[i][0])
		{
			j = echo_nl(tmp, i);
			if (j > i)
        		newline = 0;
		}
    }
    while (tmp->commands[j])
	{
		if (c == 1)
			printf (" ");
		printf ("%s", tmp->commands[j++]);
		c = 1;
	}
    if (newline)
        printf("\n");
    return (0);
}

char	*bring_this(t_env *env, char *cmp)
{
	int		i;

	i = 0;
	while (env)
	{
		while (cmp[i] && env->key[i] && cmp[i] == env->key[i])
			i++;
		if (cmp[i] == env->key[i])
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

int builtin_cd(t_cmd *tmp, t_other *other)
{
	char *home;

	if (!tmp->commands[1] || !tmp->commands[1][0])
		return (exit_status(0), 0);
	home = bring_this(other->envrp, "HOME");
	if (tmp->commands[1] && tmp->commands[1][0])
	{
		if (tmp->commands[1][0] == '~' && !tmp->commands[1][1])
		{
			if (home == NULL)
			{
				write (2, "minishell: cd: HOME not set\n", 28);
				return (1);
			}
			if (chdir(home) == -1)
				return (perror("minishell: "), 1);
			return (0);
		}
		if (chdir(tmp->commands[1]) == -1)
			return (perror("minishell: "), 1);
	}
	return (0);
}

int replace_pwd(int len, t_other *other, char *cwdp)
{
	int i;
	t_env *en;
	char *path;

	path = "PATH";
	i = 0;
	en = other->envrp;
	while (en)
	{
		while (path[i] && en->key[i] && path[i] == en->key[i])
			i++;
		if (path[i] == en->key[i])
		{
			free (en->value);
			en->value = NULL;
			en->value = malloc (len + 1);
			if (!en->value)
				return (perror("malloc: ") ,-1);
			i = 0;
			while (cwdp[i])
			{
				en->value[i] = cwdp[i];
				i++;
			}
			en->value[i] = 0;
			return (SUCCESSFUL);
		}
		en = en->next;
	}
	return (SUCCESSFUL);
}

int builtin_pwd(t_other *other)
{
	char cwd[PATH_MAX];
	char *cwdp;
	int	len;
	int	r;

	if (getcwd(cwd, sizeof(cwd)) == NULL)
		return (perror("pwd "), 1);
	len = ft_strlen(cwd);
	cwdp = cwd;
	r = replace_pwd(len, other, cwdp);
	if (r == -1)
		return (-1);
	printf("%s\n", cwd);
	return (0);
}

int listenv(t_env *en)
{
	while (en)
	{
		printf ("declare -x %s=\"%s\"\n", en->key, en->value);
		en = en->next;
	}
	return (0);
}

int check_export(char *opt)
{
	int i;

	i = 0;
	if (!opt[0])
	{
		write (2, "minishell: export: '", 20);
		write (2, "': not a valid identifier\n", 25);
		return (0);
	}
    if (!ft_isalpha(opt[0]) && opt[0] != '_')
	{
		write (2, "minishell: export: '", 20);
		write (2, opt, ft_strlen(opt));
		write (2, "': not a valid identifier\n", 25);
		return (1);
	}
    while (opt[i] && opt[i] != '=')
    {
		if (!ft_isalnum(opt[i]) && opt[i] != '_')
		{
			write (2, "minishell: export: '", 20);
			write (2, opt, ft_strlen(opt));
			write (2, "': not a valid identifier\n", 25);
			return (1);
		}
        i++;
    }
    if (opt[i] && opt[i] == '=')
    	return (0);
    return (2);
}

int	init_export(char *opt, t_other *other)
{
	int i;
	int j;
	int t;
	char *con1;
	char *con2;
	t_env *new;

	i = 0;
    while (opt[i] && opt[i] != '=')
	{
        i++;	
	}
	j = i + 1;
    while (opt[j])
	{
        j++;
	}
	con2 = NULL;
	con1 = malloc (i + 1);
	if (!con1)
		return (perror("malloc "), 1);
	t = 0;
	while (t < i)
	{
		con1[t] = opt[t];
		t++;
	}
	if ((j - i) >= i)
	{
		con2 = malloc ((j - i) + 1);
		if (!con2)
			return (perror("malloc "), free(con1), 1);
		t = 0;
		while (opt[i + 1])
		{
			con2[t] = opt[i + 1];
			i++;
			t++;
		}
	}
	new = ft_lstnew(con1, con2);
	if (new == NULL)
		return (perror("malloc "), free(con1), free(con2), 1);
	ft_lstadd_back(&other->envrp, new);
	return (0);
}
int builtin_export(t_cmd *tmp, t_other *other)
{
	int	r;
	int	i;

	i = 1;
	if (!tmp->commands[1])
		return (listenv(other->envrp), 0);
	while (tmp->commands[i])
	{
		r = check_export(tmp->commands[i]);
		if (r == 2)
			return (exit_status(0), 0);
		else if (r == 1)
			return (exit_status(1), 1);
		r = init_export(tmp->commands[i], other);
		if (r == -1)
			return (1);
		i++;
	}
	return (SUCCESSFUL);
}

int check_unset(char *opt)
{
	int i;

	i = 0;
	if (!opt[0] || opt[0] == '-')
		return (printf ("minishell: export: '%s': not a valid identifier\n", opt), 0);
    if (!ft_isalpha(opt[0]) && opt[0] != '_')
		return (printf ("minishell: export: '%s': not a valid identifier\n", opt), 1);
    while (opt[i])
    {
        if (!ft_isalnum(opt[i]) && opt[i] != '_')
            return (printf ("minishell: export: '%s': not a valid identifier\n", opt), 1);
        i++;
    }
    return (0);
}

int find_unset(char *opt, t_other *other)
{
	int		r;
	int		c;
	t_env	*tenv;
	t_env	*tempo2;

	c = 0;
	tenv = other->envrp;
	while (tenv)
	{
		r = ft_strcmp(tenv->key, opt);
		if (r == 0)
		{
			if (tenv->next == NULL)
			{
				tempo2 = other->envrp;
				while (tempo2->next->next)
					tempo2 = tempo2->next;
				free(tenv->key);
				free(tenv->value);
				free(tenv);
				tenv->key = NULL;
				tenv->value = NULL;
				tempo2->next = NULL;
			}
			else if (c == 0)
			{
				free(tenv->key);
				free(tenv->value);
				tenv->key = NULL;
				tenv->value = NULL;
				tenv = NULL;
				other->envrp = other->envrp->next;
			}
			else
			{
				tempo2 = tenv->next;
				free(tenv->key);
				free(tenv->value);
				tenv->key = NULL;
				tenv->value = NULL;
				tenv = other->envrp;
				while (tenv->next->key)
					tenv = tenv->next;
				free(tenv->next);
				tenv->next = tempo2;
				tenv->next = tempo2;
			}
			return (0);
		}
		c = 1;
		tenv = tenv->next;
	}
	return (0);
}

int builtin_unset(t_cmd *tmp, t_other *other)
{
	int i;

	i = 1;
	if (!tmp->commands[1])
		return (0);
	while (tmp->commands[i])
	{
		check_unset(tmp->commands[i]);
		find_unset(tmp->commands[i], other);
		i++;
	}
	return (0);
}

int builtin_env(t_cmd *tmp, t_other *other)
{
    t_env *tmpo;

    if (tmp->commands[1])
        return (0);
    tmpo = other->envrp;
    while (tmpo)
    {
        if (tmpo->value)
            printf("%s=%s\n", tmpo->key, tmpo->value);
        tmpo = tmpo->next;
    }
    return (0);
}

int	ft_atoi(const char *str)
{
	int					i;
	int					sign;
	unsigned long long	result;

	i = 0;
	sign = 1;
	result = 0;
	while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign *= -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		if (sign == 1 && result > 9223372036854775807)
			return (-1);
		if (sign == -1 && result > 9223372036854775807)
			return (0);
		result = (result * 10) + (str[i] - 48);
		i++;
	}
	return ((int)(result * sign));
}

int builtin_exit(t_cmd *tmp)
{
	int i;

	i = 0;
	if (!tmp->commands[1])
		return (exit_status (0), exit(1), 1);
	while (tmp->commands[1][i])
	{
		if (tmp->commands[1][i] > '9' || tmp->commands[1][i] < '0')
		{
			printf ("minishell: exit: hello: numeric argument required\n");
			return (exit_status (1), exit(1), 1);
		}
		i++;
	}
	if (tmp->commands[2])
	{
		printf ("minishell: exit: too many arguments\n");
		return (exit_status (1), 1);
	}
	exit (ft_atoi(tmp->commands[1]));
}

int run_bin(t_cmd *tmp, t_other *other)
{
	int	r;

	if (is_equal(tmp->commands[0], "echo"))
	{
		builtin_echo(tmp);
		exit_status(0);
		return (SUCCESSFUL);
	}
	else if (is_equal(tmp->commands[0], "cd"))
	{
		builtin_cd(tmp, other);
		exit_status(0);
		return (SUCCESSFUL);
	}
	else if (is_equal(tmp->commands[0], "pwd") == 1)
	{
		r = builtin_pwd(other);
		exit_status(0);
		return (r);
	}
	else if (is_equal(tmp->commands[0], "export"))
	{
		r = builtin_export(tmp, other);
		exit_status(0);
		return (r);
	}
	else if (is_equal(tmp->commands[0], "unset"))
	{
		r = builtin_unset(tmp, other);
		exit_status(0);
		return (r);
	}
	else if (is_equal(tmp->commands[0], "env"))
	{
		builtin_env(tmp, other);
		exit_status(0);
		return (SUCCESSFUL);
	}
	else if (is_equal(tmp->commands[0], "exit"))
	{
		r = builtin_exit(tmp);
		exit_status(r);
		return (r);
	}
	return (FAILED);
}

int	exec(t_cmd *tmp, t_other *other)
{
	// int i = 0;
	// while (tmp->argument[i])
		// dprintf (other->debug, "QBL arg ==== : %s\n", tmp->argument[i++]);
	if (tmp->bin == 1)
	{
		run_bin(tmp, other);
		// dprintf (other->debug, "==== \n");
		return (0);
	}
	else if (tmp->path_cmd == NULL)
	{
		restore_fds(other);
		// printf ("1............................\n");
		free_all(other);
		write (2, "Error: ", ft_strlen("Error: "));
		write (2, tmp->commands[0], ft_strlen(tmp->commands[0]));
		write (2, " command not found\n", ft_strlen(" command not found\n"));
		return (127);
	}
	// dprintf (other->debug, "HAVE A LOOK ON THE ENV : %s\n", other->envr[0]);
	// while (1);
	// printf ("FULL PATH of tmp ==== : %s\n", tmp->path_cmd);
	// printf ("FULL PATH of other ==== : %s\n", other->orig_cmd->path_cmd);
	// printf ("cmd of tmp ==== : %s\n", tmp->commands[0]);
	// printf ("cmd of other ==== : %s\n", other->orig_cmd->commands[0]);
	// printf ("here\n");
	// while (1);
	if (execve(tmp->path_cmd, tmp->argument, other->envr) == ERROR)
	{
		// printf ("2............................\n");
		free_all(other);
		perror ("execve: ");
		exit_status(1);
		exit(1);
	}
	return (SUCCESSFUL);
}

int	dupping(t_cmd *tmp, t_other *other)
{
	t_ind	ind;

	ind.r = -99;
	nothing(other);
	if (tmp->open1 != -3 && tmp->open1 != -1)
	{
		ind.r = dup2(tmp->open1, 0);
		other->stdin_flag = 1;
		if (ind.r == -1)
			return (ERROR);
		close(tmp->open1);
		tmp->open1 = -3;
	}
	if (tmp->open2 != -3 && tmp->open2 != -1)
	{
		ind.r = dup2(tmp->open2, 1);
		other->stdout_flag = 1;
		if (ind.r == -1)
			return (ERROR);
		close(tmp->open2);
		tmp->open2 = -3;
	}
	close_fds(tmp);
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
				close(tmp->open1);
				tmp->open1 = -3;
			}
			tmp->open1 = tmp->pipedoc[READ];
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
	nothing (other);
	// ◦ echo with option -n
	// ◦ cd with only a relative or absolute path
	// ◦ pwd with no options
	// ◦ export with no options
	// ◦ unset with no options
	// ◦ env with no options or arguments
	// ◦ exit with no options
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
		return (cmd->bin = 1, 0);
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
	{
		// printf ("3............................\n");
		return (printf("argument failed !\n"), free_all(other), ERROR);
	}
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
		restore_fds(other);
		close_all_fds(tmp);
		return (1);
	}
	// printf ("......\n");
	// while (1);
	if (!tmp->commands[0])
		return (0);
	ind.r = dupping(tmp, other);
	if (ind.r == -1)
	{
		restore_fds(other);
		close_all_fds(tmp);
		return (perror("Error dup2: "), 1);
	}
	// while (1);
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

int	pipping(t_cmd *tmp, int type)
{
	t_ind	ind;

	if (type == 1)
	{
		ind.r = pipe(tmp->pipefd);
	}
	else
	{
		ind.r = pipe(tmp->pipedoc);
	}
	if (ind.r == -1)
	{
		// printf ("4............................\n");
		// free_all(other);
		perror ("Error: pipe failed\n");
		return (ERROR);
	}
	return (SUCCESSFUL);
}

int	make_heredoc(t_cmd *tmp, t_other *other, char *limmiter)
{
	char	*line;
	t_ind	ind;

	ind.c = 0;
	nothing(other);
	while (1)
	{
		write (1, "> ", 2);
		line = get_next_line(0);
		if (line == NULL && ind.c == 0)
		{
			close(tmp->pipedoc[READ]);
			tmp->pipedoc[READ] = -3;
			close(tmp->pipedoc[WRITE]);
			tmp->pipedoc[WRITE] = -3;
			if (tmp->flag_exit == 0)
				exit_status(1);
			return (1);
		}
		if (line == NULL && ind.c == 1)
			break ;
		ind.c = 1;
		if (is_limiter(line, limmiter) == TRUE)
		{
			close(tmp->pipedoc[WRITE]);
			tmp->pipedoc[WRITE] = -3;
			return (free(line), SUCCESSFUL);
		}
		if (tmp->count_doc == 0 && tmp->red->expand == 0)
        	write (tmp->pipedoc[WRITE], line, mystrlen(line));
        if (tmp->count_doc == 0 && tmp->red->expand == 1)
		{
        	resolve_heredoc(other->envrp, &line, tmp->pipedoc[WRITE]);
		}
		free(line);
	}
	if (line)
		free(line);
	close(tmp->pipedoc[WRITE]);
	tmp->pipedoc[WRITE] = -3;
	close(tmp->pipedoc[READ]);
	tmp->pipedoc[READ] = -3;
	return (SUCCESSFUL);
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
		// printf ("hna\n");
		set_signals_child();
		restore_fds(other);
		ind.r = child_process(tmp, other, i);
		close_all_fds(other->orig_cmd);
		exit (ind.r);
	}
	// while (1);
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
	if (tmp->pipedoc[READ] != -3)
	{
		close(tmp->pipedoc[READ]);
		tmp->pipedoc[READ] = -3;
	}
	if (tmp->pipedoc[WRITE] != -3)
	{
		close(tmp->pipedoc[WRITE]);
		tmp->pipedoc[WRITE] = -3;
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

int work3(t_cmd *tmp, t_other *other)
{
	int	i;
	int	r;
	int	last;

	i = 0;
	while (tmp)
	{
		if (tmp->next)
		{
			r = pipping(tmp, 1);
			if (r == ERROR)
			{
				restore_fds(other);
				exit_status(1);
				return (close_all_fds(other->orig_cmd), 1);
			}
		}
		// printf ("hna\n");
		// while (1);
		tmp->pid = fork();
		last = tmp->pid;
		r = execution2(tmp, other, i);
		if (r == ERROR)
		{
			restore_fds(other);
			return (0);
		}
		i++;
		tmp = tmp->next;
	}
	tmp = other->orig_cmd;
	while (i--)
	{
		r = wait(&other->exit_status);
		if (r == last)
		{
			handle_exit_status(other->exit_status);
		}
	}
	// printf ("ha huwa hna\n");
	// printf ("cmd with path : %s\n", other->orig_cmd->path_cmd);
	// while (1);
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
int  work(t_cmd *cmd, t_other *other)
{
	t_cmd	*tmp;
	t_ind	ind;

	ind.i = 0;
	tmp = cmd;
	while (tmp)
	{
		set_up(tmp);
		ind.r = count_heredoc(tmp);
		if (ind.r > 0)
		{
			ind.r = pipping(tmp, 2);
			if (ind.r == ERROR)
			{
				restore_fds(other);
				if (tmp->flag_exit == 0)
					return (exit_status(1), 1);
				return (1);
			}
			ind.r = fork ();
			ind.c = child_doc(tmp, other, &ind);
			if (ind.c == FAILED)
				return (FAILED);
		}
		// while (1);
		if (tmp->commands[0])
		{
			ind.c = check_cmd(tmp, other);
			if (ind.c == ERROR)
			{
				restore_fds(other);
				if (tmp->flag_exit == 0)
				exit_status(1);
				close_all_fds(other->orig_cmd);
				return (1);
			}
			count_args(tmp);
			ind.r = fill_argument(tmp, other);
			if (ind.r == ERROR)
			{
				restore_fds(other);
				exit_status(1);
				close_all_fds(other->orig_cmd);
				return (1); 
			}
		}
		ind.i++;
		tmp = tmp->next;
	}
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
		ind.r = work3(tmp, other);
		if (ind.r == 1)
		{
			restore_fds(other);
			return (close_all_fds(cmd), 1);
		}
	}
	return (SUCCESSFUL);
}

int	find_path(t_other *other, t_env *env)
{
	t_env	*tmp;

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
		// printf ("6............................\n");
		free_all(other);
		exit_status(1);
		perror ("malloc failed: \n");
		restore_fds(other);
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
	{
		exit_status(1);
		restore_fds(other);
		why_exit("other->paths allocation failed !", FAILED);
	}
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

	other->stdout_copy = dup(STDOUT_FILENO);
	other->stdin_copy = dup(STDIN_FILENO);
	other->stdin_flag = 0;
	other->stdout_flag = 0;
	while (cmd)
	{
		cmd->argument = NULL;
		cmd->path_cmd = NULL;
		cmd = cmd->next;
	}
	other->envr = NULL;
	other->paths = NULL;
}

int execution(t_cmd *cmd, t_env *env, char **ev)
{
	t_other	other;
	// t_env *tmp;
	// tmp = env;

	// test(cmd);
	signal(SIGINT, SIG_IGN);
	other.envr = ev;
	other.envrp = env;
	other.orig_cmd = cmd;
	is_pipe(cmd, &other);
	edit_paths(&other, env);
	work(cmd, &other);
	restore_fds(&other);
	// while (1);
	close_all_fds(cmd);
	// printf ("kistna! \n");
	// while (1);
	// printf ("CAME FROM HERE \n");
	free_all(&other);
	free_env(&other);
	return (0);
}
