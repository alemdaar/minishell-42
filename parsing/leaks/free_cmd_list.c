/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_cmd_list.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelhasso <oelhasso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 18:05:40 by oelhasso          #+#    #+#             */
/*   Updated: 2025/09/15 19:55:02 by oelhasso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	free_redirections(t_red *red)
{
	t_red	*tmp;

	while (red)
	{
		tmp = red;
		free_string(&red->file);
		red = red->next;
		free(tmp);
	}
}

void	cmdfree(t_cmd *cmd)
{
	t_cmd	*tmp;

	while (cmd)
	{
		tmp = cmd;
		if (cmd->commands)
			p2char(&cmd->commands);
		if (cmd->red)
			free_redirections(cmd->red);
		cmd = cmd->next;
		free(tmp);
	}
}
