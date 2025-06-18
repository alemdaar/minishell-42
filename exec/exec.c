/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelhasso <oelhasso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 17:26:38 by oelhasso          #+#    #+#             */
/*   Updated: 2025/06/18 21:24:56 by oelhasso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "exec.h"

// void test(t_cmd *cmd)
// {
// 	int i;

// 	i = 0;
// 	while (cmd)
// 	{
// 		printf ("1---\n");
// 		i = 0;
// 		while (cmd->commands[i])
// 			printf ("cmd : %s\n", cmd->commands[i++]);
// 		while (cmd->red)
// 		{
// 			printf ("file : %s\n", cmd->red->file);
// 			printf ("type : %u\n", cmd->red->red_type);
// 			cmd->red = cmd->red->next;
// 		}
// 		cmd = cmd->next;
// 		printf ("2---\n");
// 	}
// }

int is_pipe(t_cmd *cmd, t_info *dainfo)
{
	int	i;

	i = 0;
	while (cmd)
	{
		i++;
		cmd = cmd->next;
	}
	if (i > 1)
		dainfo->a_pipe = 1;
	else
		dainfo->a_pipe = 0;
	return 0;
}

int exec (t_cmd *cmd, t_env *env)
{
	t_info *dainfo;
	// t_env *tmp;
	// tmp = env;

	// test(cmd);
	is_pipe(cmd, dainfo);
	while (cmd)
	{
		dupping();
		
		cmd = cmd->commands;
	}
	return (0);
}