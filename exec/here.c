/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelhasso <oelhasso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 11:20:38 by oelhasso          #+#    #+#             */
/*   Updated: 2025/09/18 22:48:18 by oelhasso         ###   ########.fr       */
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



