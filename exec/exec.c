/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelhasso <oelhasso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 17:26:38 by oelhasso          #+#    #+#             */
/*   Updated: 2025/06/17 22:09:29 by oelhasso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void test(t_cmd *cmd)
{
	int i;

	i = 0;
	while (cmd)
	{
		printf ("1---\n");
		i = 0;
		while (cmd->commands[i])
			printf ("cmd : %s\n", cmd->commands[i++]);
		while (cmd->red)
		{
			printf ("file : %s\n", cmd->red->file);
			printf ("type : %u\n", cmd->red->red_type);
			cmd->red = cmd->red->next;
		}
		cmd = cmd->next;
		printf ("2---\n");
	}
}

int exec (t_cmd *cmd, t_env *env)
{
	printf ("env : %s\n", env->key);
	return 0;
}