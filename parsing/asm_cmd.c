/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asm_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelhasso <oelhasso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 15:09:31 by mbarhoun          #+#    #+#             */
/*   Updated: 2025/06/17 21:23:08 by oelhasso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_cmd	*assemble_command(char *input, t_env *env)
{
	t_cmd	*cmd;
	t_token	*tokens;

	tokens = NULL;
	if (!quotes_is_valid(input))
		return (first_free(tokens, input), printf(ERR_QUOTES), NULL);
	tokens = segment_input(input);
	if (!tokens)
		return (p1char(&input), NULL);
	refactor_tokens(&tokens, env);
	if (!all_scrap(tokens))
		return (first_free(tokens, input), NULL);
	cmd = create_list_cmd(tokens);
	if (!hydrate_cmd(&cmd, tokens))
		return (first_free(tokens, input), NULL);
	// first_free(tokens, input);
	// int	r = 0;
	// while (cmd)
	// {
	// 	printf("COMMAND [%d]\n", r++);
	// 	int i = -1;
	// 	while (cmd->commands[++i])
	// 		printf("c = %s\n", cmd->commands[i]);
	// 	while (cmd->red)
	// 	{
	// 		printf("file is %s\n", cmd->red->file);
	// 		cmd->red = cmd->red->next;
	// 	}
	// 	cmd = cmd->next;
	// }
	return (cmd);
}
