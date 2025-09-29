/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paths.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelhasso <oelhasso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 11:27:14 by oelhasso          #+#    #+#             */
/*   Updated: 2025/09/29 17:14:56 by oelhasso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "./header.h"

static int	find_path(t_other *other, t_env *env)
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

static void	count_path(t_other *other)
{
	t_ind	ind;

	other->count_path = 0;
	if (other->all_path == NULL)
		return ;
	ind.i = 0;
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

static void	fill_path(t_other *other, t_ind *ind)
{
	int	a;

	a = 0;
	other->paths[ind->c] = malloc (sizeof(char) * (ind->j - ind->i) + 1);
	if (!other->paths[ind->c])
	{
		free_all(other);
		restore_fds(other);
		why_exit(ERR_MEM, FAILED);
	}
	while (ind->i < ind->j)
		other->paths[ind->c][a++] = other->all_path[ind->i++];
	other->paths[ind->c][a] = 0;
	return ;
}

int	edit_paths(t_other *other, t_env *env)
{
	t_ind	ind;

	find_path(other, env);
	count_path(other);
	other->paths = NULL;
	if (other->all_path == NULL)
		return (0);
	other->paths = malloc (sizeof(char *) * other->count_path);
	if (!other->paths)
		return (restore_fds(other), why_exit(ERR_MEM, 1), 1);
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
	return (0);
}
