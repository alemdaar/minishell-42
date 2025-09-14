/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelhasso <oelhasso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 18:03:00 by oelhasso          #+#    #+#             */
/*   Updated: 2025/09/14 18:03:01 by oelhasso         ###   ########.fr       */
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
		return (eprintf(ERR_PIPE), NULL);
	cmd = cmd_node();
	if (!cmd)
		return (eprintf(ERR_MEM), NULL);
	tmp = cmd;
	var.r = 0;
	while (++var.r < var.pipe)
	{
		new = cmd_node();
		if (!new)
			return (eprintf(ERR_MEM), NULL);
		tmp->next = new;
		new->prev = tmp;
		tmp = new;
	}
	return (cmd);
}
