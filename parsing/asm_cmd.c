/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asm_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelhasso <oelhasso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 18:06:39 by oelhasso          #+#    #+#             */
/*   Updated: 2025/09/25 20:37:29 by oelhasso         ###   ########.fr       */
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
		printf ("check quotes\n");
		exit_status(258);
		return (free_parcing(tokens, input), fprintf(stderr, ERR_QUOTE), NULL);
	}
	printf ("env space\n");
	env_space(&input, env);
	printf ("seg\n");
	tokens = segment_input(input);
	if (!tokens)
		return (free_string(&input), NULL);
	printf ("all scrap\n");
	if (!all_scrap(tokens))
		return (free_parcing(tokens, input), exit_status(258), NULL);
	printf ("refactor_tokens\n");
	refactor_tokens(&tokens, env);
	printf ("create_list_cmd\n");
	cmd = create_list_cmd(tokens);
	if (!hydrate_cmd(&cmd, tokens))
		return (free_parcing(tokens, input), NULL);
	free_parcing(tokens, input);
	return (cmd);
}
