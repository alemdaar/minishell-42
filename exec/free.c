/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelhasso <oelhasso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 11:20:38 by oelhasso          #+#    #+#             */
/*   Updated: 2025/09/29 18:15:59 by oelhasso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "header.h"

void	free_env(t_other *other)
{
	t_env	*tmp;

	if (other->envrp)
	{
		while (other->envrp)
		{
			if (other->envrp->key)
			{
				free(other->envrp->key);
				other->envrp->key = NULL;
			}
			if (other->envrp->value)
			{
				free(other->envrp->value);
				other->envrp->value = NULL;
			}
			tmp = other->envrp;
			other->envrp = tmp->next;
			free(tmp);
			tmp = NULL;
		}
	}
	return ;
}

static void	free_all3(t_other *other)
{
	t_cmd	*tmp;
	t_red	*red_copy;
	t_red	*red_prev;

	tmp = other->orig_cmd;
	red_copy = tmp->red;
	if (red_copy)
	{
		while (red_copy)
		{
			if (red_copy->file)
			{
				free(red_copy->file);
				red_copy->file = NULL;
			}
			red_prev = red_copy;
			red_copy = red_copy->next;
			free(red_prev);
		}
	}
	return ;
}

static void	free_all2(t_other *other)
{
	int	i;

	if (other->orig_cmd->commands)
	{
		free(other->orig_cmd->commands);
		other->orig_cmd->commands = NULL;
	}
	if (other->orig_cmd->path_cmd)
	{
		free(other->orig_cmd->path_cmd);
		other->orig_cmd->path_cmd = NULL;
	}
	i = 0;
	if (other->orig_cmd->argument)
	{
		while (other->orig_cmd->argument[i])
		{
			free(other->orig_cmd->argument[i]);
			other->orig_cmd->argument[i++] = NULL;
		}
		free(other->orig_cmd->argument);
		other->orig_cmd->argument = NULL;
	}
	free_all3(other);
	return ;
}

int	free_commands(t_other *other)
{
	t_cmd	*tmp;

	while (other->orig_cmd)
	{
		tmp = other->orig_cmd;
		free_all2(other);
		other->orig_cmd = tmp->next;
		free(tmp);
	}
	return (SUCCESSFUL);
}

int	free_all(t_other *other)
{
	int		i;

	i = 0;
	while (i < other->count_path && other->paths)
	{
		if (other->paths[i] == NULL)
		{
			free(other->paths);
			return (other->paths = NULL, FAILED);
		}
		free(other->paths[i]);
		other->paths[i] = NULL;
		i++;
	}
	if (other->paths == NULL)
	{
		return (FAILED);
	}
	else
	{
		free(other->paths);
		other->paths = NULL;
	}
	free_commands(other);
	return (0);
}
