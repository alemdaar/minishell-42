/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nodes_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelhasso <oelhasso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 18:03:06 by oelhasso          #+#    #+#             */
/*   Updated: 2025/09/24 12:06:27 by oelhasso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_red	*red_node(void)
{
	t_red	*red;

	red = malloc(sizeof(t_red));
	if (!red)
		return (NULL);
	red->next = NULL;
	return (red);
}

t_cmd	*cmd_node(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->red = NULL;
	cmd->prev = NULL;
	cmd->next = NULL;
	cmd->amb = 0;
	return (cmd);
}
