/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fds.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelhasso <oelhasso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 20:33:28 by oelhasso          #+#    #+#             */
/*   Updated: 2025/09/21 20:33:54 by oelhasso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "./header.h"

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
		close_set(&tmp->open1, -3);
	if (tmp->open2 != -3)
		close_set(&tmp->open2, -3);
	if (tmp->pipefd[READ] != -3)
		close_set(&tmp->pipefd[READ], -3);
	if (tmp->pipefd[WRITE] != -3)
		close_set(&tmp->pipefd[WRITE], -3);
	if (tmp->pipedoc[READ] != -3)
		close_set(&tmp->pipedoc[READ], -3);
	if (tmp->pipedoc[WRITE] != -3)
		close_set(&tmp->pipedoc[WRITE], -3);
	return ;
}

void close_all_fds(t_cmd *cmd)
{
	while (cmd)
	{
		if (cmd->open1 != -3)
			close_set(&cmd->open1, -3);
		if (cmd->open2 != -3)
			close_set(&cmd->open2, -3);
		if (cmd->pipefd[READ] != -3)
			close_set(&cmd->pipefd[READ], -3);
		if (cmd->pipefd[WRITE] != -3)
			close_set(&cmd->pipefd[WRITE], -3);
		if (cmd->pipedoc[READ] != -3)
			close_set(&cmd->pipedoc[READ], -3);
		if (cmd->pipedoc[WRITE] != -3)
			close_set(&cmd->pipedoc[WRITE], -3);
		cmd = cmd->next;
	}
}
