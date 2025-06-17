/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_cmd_list.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbarhoun <mbarhoun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 10:51:25 by mbarhoun          #+#    #+#             */
/*   Updated: 2025/06/01 13:02:06 by mbarhoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	free_redirections(t_red *red)
{
	t_red	*tmp;

	while (red)
	{
		tmp = red;
		p1char(&red->file);
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
