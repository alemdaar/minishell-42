/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelhasso <oelhasso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 12:15:53 by oelhasso          #+#    #+#             */
/*   Updated: 2025/09/25 13:38:27 by oelhasso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "./header.h"

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
	if (!tmp->commands[0])
		return (0);
	ind.r = dupping(tmp, other);
	if (ind.r == -1)
	{
		restore_fds(other);
		close_all_fds(tmp);
		return (perror("Error dup2: "), 1);
	}
	ind.r = exec(tmp, other);
	return (ind.r);
}
