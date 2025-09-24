/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asm_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelhasso <oelhasso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 18:06:39 by oelhasso          #+#    #+#             */
/*   Updated: 2025/09/24 12:54:15 by oelhasso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../exec//header.h"

t_cmd	*parcing(char *input, t_env *env)
{
	t_cmd	*cmd;
	t_token	*tokens;

	tokens = NULL;
	if (!check_quotes(input))
	{
		exit_status(258);
		return (free_parcing(tokens, input), fprintf(stderr, ERR_QUOTE), NULL);
	}
	env_space(&input, env);
	tokens = segment_input(input);
	if (!tokens)
		return (free_string(&input), NULL);
	if (!all_scrap(tokens))
		return (free_parcing(tokens, input), exit_status(258), NULL);
	refactor_tokens(&tokens, env);
	cmd = create_list_cmd(tokens);
	if (!hydrate_cmd(&cmd, tokens))
		return (free_parcing(tokens, input), NULL);
	free_parcing(tokens, input);
	return (cmd);
}
