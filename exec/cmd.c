/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelhasso <oelhasso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 12:41:56 by oelhasso          #+#    #+#             */
/*   Updated: 2025/09/18 12:55:25 by oelhasso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./header.h"
#include "../minishell.h"

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
	if (access(cmd->path_cmd, F_OK) == SUCCESSFUL)
	{
		if (access(cmd->path_cmd, X_OK) == SUCCESSFUL)
			return (TRUE);
	}
	return (free(cmd->path_cmd), cmd->path_cmd = NULL, FALSE);
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