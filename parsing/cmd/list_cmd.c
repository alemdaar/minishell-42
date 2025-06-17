/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelhasso <oelhasso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 15:06:10 by mbarhoun          #+#    #+#             */
/*   Updated: 2025/06/17 21:39:44 by oelhasso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_cmd	*create_list_cmd(t_token *tokens)
{
	t_cmd	*cmd;
	t_cmd	*new;
	t_cmd	*tmp;
	t_norm	var;

	var.pipe = counter_pipe(tokens);
	if (!var.pipe)
		return (printf(ERR_PIPE), NULL);
	cmd = cmd_node();
	if (!cmd)
		return (NULL);
	tmp = cmd;
	var.r = 0;
	while (++var.r < var.pipe)
	{
		new = cmd_node();
		if (!new)
			return (NULL);
		tmp->next = new;
		new->prev = tmp;
		tmp = new;
	}
	return (cmd);
}
