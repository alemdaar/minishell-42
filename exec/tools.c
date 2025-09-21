/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelhasso <oelhasso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 12:42:53 by oelhasso          #+#    #+#             */
/*   Updated: 2025/09/21 22:48:43 by oelhasso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "./header.h"

int is_builtin(t_cmd *tmp)
{
	if (is_equal(tmp->commands[0], "echo"))
		return (tmp->bin = 1, SUCCESSFUL);
	if (is_equal(tmp->commands[0], "cd"))
		return (tmp->bin = 1, SUCCESSFUL);
	if (is_equal(tmp->commands[0], "pwd"))
		return (tmp->bin = 1, SUCCESSFUL);
	if (is_equal(tmp->commands[0], "export"))
		return (tmp->bin = 1, SUCCESSFUL);
	if (is_equal(tmp->commands[0], "unset"))
		return (tmp->bin = 1, SUCCESSFUL);
	if (is_equal(tmp->commands[0], "env"))
		return (tmp->bin = 1, SUCCESSFUL);
	if (is_equal(tmp->commands[0], "exit"))
		return (tmp->bin = 1, SUCCESSFUL);
	return (FAILED);
}
