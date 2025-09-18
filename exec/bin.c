/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bin.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelhasso <oelhasso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 19:53:07 by oelhasso          #+#    #+#             */
/*   Updated: 2025/09/18 20:13:17 by oelhasso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "./header.h"

int run_bin(t_cmd *tmp, t_other *other)
{
	int	r;

	if (is_equal(tmp->commands[0], "echo"))
		return (builtin_echo(tmp), exit_status(0), 0);
	else if (is_equal(tmp->commands[0], "cd"))
		return (r = builtin_cd(tmp, other), exit_status(r), r);
	else if (is_equal(tmp->commands[0], "pwd") == 1)
		return (builtin_pwd(other), exit_status(0), 0);
	else if (is_equal(tmp->commands[0], "export"))
		return (r = builtin_export(tmp, other), exit_status(r), r);
	else if (is_equal(tmp->commands[0], "unset"))
		return (r = builtin_unset(tmp, other), exit_status(r), r);
	else if (is_equal(tmp->commands[0], "env"))
		return (builtin_env(tmp, other), exit_status(0), 0);
	else if (is_equal(tmp->commands[0], "exit"))
		return (r = builtin_exit(tmp), exit_status(r), r);
	return (ERROR);
}
