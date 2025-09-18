/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelhasso <oelhasso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 11:20:38 by oelhasso          #+#    #+#             */
/*   Updated: 2025/09/18 12:56:06 by oelhasso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "header.h"
#include "gnl/get_next_line.h"

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
	exit_status(flag);
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
    return (SUCCESSFUL);
}

int replace_pwd(int len, t_other *other, char *cwdp)
{
	int i;
	t_env *en;
	char *path;

	path = "PWD";
	i = 0;
	en = other->envrp;
	while (en)
	{
		while (path[i] && en->key[i] && path[i] == en->key[i])
			i++;
		if (path[i] == en->key[i] && !path[i] && !en->key[i])
		{
			if (en->value)
			{
				
				free (en->value);
				en->value = NULL;
			}
			
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
        if (en->value)
            printf("declare -x %s=\"%s\"\n", en->key, en->value);
        else
            printf("declare -x %s\n", en->key);
        en = en->next;
    }
    return (0);
}

char *extract_value(t_env *env, const char *key)
{
    while (env)
    {
        if (ft_strcmp(env->key, key) == 0)
            return env->value;
        env = env->next;
    }
    return NULL;
}

int update_env(t_env *env, const char *key, const char *value)
{
    t_env *tmp = env;
    while (tmp)
    {
        if (ft_strcmp(tmp->key, key) == 0)
        {
            free(tmp->value);
            tmp->value = ft_strdup(value);
            if (!tmp->value) return perror("malloc"), -1;
            return SUCCESSFUL;
        }
        tmp = tmp->next;
    }
    t_env *new_node = malloc(sizeof(t_env));
    if (!new_node) return perror("malloc"), -1;
    new_node->key = ft_strdup(key);
    new_node->value = ft_strdup(value);
    new_node->next = NULL;
    if (!new_node->key || !new_node->value) return perror("malloc"), -1;

    tmp = env;
    while (tmp->next) tmp = tmp->next;
    tmp->next = new_node;
    return SUCCESSFUL;
}

int builtin_cd(t_cmd *cmd, t_other *other)
{
    char *target;
    char *oldpwd;
    char cwd[PATH_MAX];
    t_env *env = other->envrp;

    if (!getcwd(cwd, sizeof(cwd))) return perror("getcwd"), 1;
    oldpwd = ft_strdup(cwd);
    if (!oldpwd) return perror("malloc"), 1;

    if (!cmd->commands[1] || (cmd->commands[1][0] == '~' && !cmd->commands[1][1]))
    {
        target = extract_value(env, "HOME");
        if (!target)
        {
            fprintf(stderr, "minishell: cd: HOME not set\n");
            free(oldpwd);
            return 1;
        }
    }
    else if (ft_strcmp(cmd->commands[1], "-") == 0)
    {
        target = extract_value(env, "OLDPWD");
        if (!target)
        {
            fprintf(stderr, "minishell: cd: OLDPWD not set\n");
            free(oldpwd);
            return 1;
        }
        printf("%s\n", target);
    }
    else
        target = cmd->commands[1];

    if (chdir(target) == -1)
    {
        perror("minishell: cd");
        free(oldpwd);
        return 1;
    }

    if (update_env(env, "OLDPWD", oldpwd) == -1) { free(oldpwd); return 1; }
    free(oldpwd);

    if (!getcwd(cwd, sizeof(cwd))) return perror("getcwd"), 1;
    if (update_env(env, "PWD", cwd) == -1) return 1;

    return 0;
}

int	check_export(char *opt)
{
	int	i;

	// printf ("opt : %s\n", opt);
	if (!opt || !opt[0])
	{
		fprintf(stderr, "minishell: export: `%s': not a valid identifier\n", opt);
		// while (1);
		return (1);
	}
	if (!ft_isalpha(opt[0]) && opt[0] != '_')
	{
		fprintf(stderr, "minishell: export: `%s': not a valid identifier\n", opt);
		return (1);
	}
	i = 1;
	while (opt[i] && opt[i] != '=')
	{
		if (!ft_isalnum(opt[i]) && opt[i] != '_')
		{
			fprintf(stderr, "minishell: export: `%s': not a valid identifier\n", opt);
			return (1);
		}
		i++;
	}
	return (0);
}

int init_export(char *opt, t_other *other)
{
    int i = 0;
    while (opt[i] && opt[i] != '=')
        i++;

    char *key = ft_substr(opt, 0, i);
    if (!key)
        return (perror("malloc "), 1);

    char *value = NULL;
    if (opt[i] == '=')
    {
        value = ft_strdup(opt + i + 1);
        if (!value)
            return (perror("malloc "), free(key), 1);
    }

    t_env *cur = other->envrp;
    while (cur)
    {
        if (ft_strcmp(cur->key, key) == 0)
        {
            if (opt[i] == '=')
            {
                free(cur->value);
                cur->value = value;
            }
            free(key);
            return (0);
        }
        cur = cur->next;
    }

    t_env *new = ft_lstnew(key, value);
    if (!new)
        return (perror("malloc "), free(key), free(value), 1);
    ft_lstadd_back(&other->envrp, new);
    return (0);
}


int	builtin_export(t_cmd *tmp, t_other *other)
{
	int	r;
	int	i;
	int	ret;

	if (!tmp->commands[1])
		return (listenv(other->envrp), 0);
	ret = 0;
	i = 1;
	while (tmp->commands[i])
	{
		r = check_export(tmp->commands[i]);
		if (r == 0)
		{
			if (init_export(tmp->commands[i], other) != 0)
				ret = 1;
		}
		else
			ret = 1;
		i++;
	}
	exit_status(ret);
	return (ret);
}

int	check_unset(char *opt)
{
	int i;

	if (!opt || !opt[0])
	{
		write(2, "minishell: unset: '", 19);
		write(2, opt ? opt : "", ft_strlen(opt ? opt : ""));
		write(2, "': not a valid identifier\n", 26);
		return (1);
	}
	if (!ft_isalpha(opt[0]) && opt[0] != '_')
	{
		write(2, "minishell: unset: '", 19);
		write(2, opt, ft_strlen(opt));
		write(2, "': not a valid identifier\n", 26);
		return (1);
	}
	i = 1;
	while (opt[i])
	{
		if (!ft_isalnum(opt[i]) && opt[i] != '_')
		{
			write(2, "minishell: unset: '", 19);
			write(2, opt, ft_strlen(opt));
			write(2, "': not a valid identifier\n", 26);
			return (1);
		}
		i++;
	}
	return (0);
}

void	remove_env_node(t_env **head, char *key)
{
	t_env *cur = *head;
	t_env *prev = NULL;

	while (cur)
	{
		if (ft_strcmp(cur->key, key) == 0)
		{
			if (prev)
				prev->next = cur->next;
			else
				*head = cur->next;

			free(cur->key);
			free(cur->value);
			free(cur);
			return;
		}
		prev = cur;
		cur = cur->next;
	}
}

int	builtin_unset(t_cmd *tmp, t_other *other)
{
	int i;
	int status = 0;

	i = 1;
	while (tmp->commands[i])
	{
		if (check_unset(tmp->commands[i]) == 0)
			remove_env_node(&other->envrp, tmp->commands[i]);
		else
			status = 1;
		i++;
	}
	exit_status(status);
	return (status);
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

    printf("exit\n");

    if (!tmp->commands[1])
        exit(exit_status(-1));
    i = 0;
    if (!tmp->commands[1][0])
	{
		fprintf(stderr, "minishell: exit: %s: numeric argument required\n", tmp->commands[1]);
		exit(255);
	}
    while (tmp->commands[1][i] == ' ')
        i ++;
    if (tmp->commands[1][i] == '+' || tmp->commands[1][i] == '-')
        i ++;
	if (!ft_isdigit(tmp->commands[1][i]))
	{
		fprintf(stderr, "minishell: exit: %s: numeric argument required\n", tmp->commands[1]);
		exit(255);
	}
    while (tmp->commands[1][i])
    {		
		if (tmp->commands[1][i] == ' ')
		{
			while (tmp->commands[1][i] == ' ')
				i++;
			break;
		}
        if (!ft_isdigit(tmp->commands[1][i]) && tmp->commands[1][i])
        {
            fprintf(stderr, "minishell: exit: %s: numeric argument required\n", tmp->commands[1]);
            exit(255);
        }
        i++;
    }
	while (tmp->commands[1][i] == ' ')
		break;
	if (tmp->commands[1][i] != 0)
	{
		fprintf(stderr, "minishell: exit: %s: numeric argument required\n", tmp->commands[1]);
		exit(255);
	}
    if (tmp->commands[2])
    {
        fprintf(stderr, "minishell: exit: too many arguments\n");
        exit_status(1);
        return (1);
    }

    exit((unsigned char)ft_atoi(tmp->commands[1]));
}


int run_bin(t_cmd *tmp, t_other *other) 
{
	int	r;

	if (is_equal(tmp->commands[0], "echo"))
	{
		builtin_echo(tmp);
		exit_status(SUCCESSFUL);
		return (SUCCESSFUL);
	}
	else if (is_equal(tmp->commands[0], "cd"))
	{
		r = builtin_cd(tmp, other);
		exit_status(r);
		return (r);
	}
	else if (is_equal(tmp->commands[0], "pwd") == 1)
	{
		builtin_pwd(other);
		exit_status(SUCCESSFUL);
		return (SUCCESSFUL);
	}
	else if (is_equal(tmp->commands[0], "export"))
	{
		r = builtin_export(tmp, other);
		exit_status(r);
		return (r);
	}
	else if (is_equal(tmp->commands[0], "unset"))
	{
		r = builtin_unset(tmp, other);
		exit_status(r);
		return (r);
	}
	else if (is_equal(tmp->commands[0], "env"))
	{
		builtin_env(tmp, other);
		exit_status(SUCCESSFUL);
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
	int r = 0;
	if (tmp->bin == 1)
	{
		r = run_bin(tmp, other);
		
		return (r);
	}
	if (!tmp->commands[0] || tmp->commands[0][0] == '\0')
	{
	    restore_fds(other);
	    write(2, "minishell: ", 11);
	    write(2, ": command not found\n", 20);
	    free_all(other);
	    return (127);
	}

	if (ft_strchr(tmp->commands[0], '/') != -1)
	{
		
		if (stat(tmp->commands[0], &st) == -1)
		{
			fprintf(stderr, "minishell: %s: No such file or directory", tmp->commands[0]);
			exit(127);
		}
		
		if (S_ISDIR(st.st_mode))
		{
			fprintf(stderr, "minishell: %s: is a directory\n", tmp->commands[0]);
			exit(126);
		}
		
		if (access(tmp->commands[0], X_OK) == -1)
		{
			fprintf(stderr, "minishell: %s: Permission denied\n", tmp->commands[0]);
			exit(126); 
		}
		execve(tmp->commands[0], tmp->argument, other->envr);
		perror("minishell");
		exit(126); 
	}
	
	if (tmp->path_cmd == NULL) 
	{
		restore_fds(other);
		write(2, "minishell: ", 11);
		write(2, tmp->commands[0], ft_strlen(tmp->commands[0]));
		write(2, ": command not found\n", 20);
		free_all(other);
		return (127);
	}
	execve(tmp->path_cmd, tmp->argument, other->envr);
	perror("minishell");
	exit(126);
	return (SUCCESSFUL);
}

int	dupping(t_cmd *tmp, t_other *other)
{
	t_ind	ind;

	ind.r = -99;
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
