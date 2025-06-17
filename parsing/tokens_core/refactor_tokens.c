/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   refactor_tokens.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbarhoun <mbarhoun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 18:10:51 by mbarhoun          #+#    #+#             */
/*   Updated: 2025/06/01 12:00:46 by mbarhoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	demote_env_token(t_token **tokens)
{
	t_token	*token;

	token = *tokens;
	while (token)
	{
		if (token->type_token == ENV)
			token->type_token = WORD;
		token = token->next;
	}
}

void	refactor_tokens(t_token **tokens, t_env *env)
{
	t_token	*token;
	bool	expander;

	token = *tokens;
	expander = 1;
	while (token)
	{
		is_env(&token->content, env, expander);
		if (has_quotes(token->content))
		{
			if (!expander)
				token->exp = 0;
			token->content = remove_quotes(token->content);
		}
		expander = 1;
		if (token->type_token == HERDOOC)
			expander = 0;
		token = token->next;
	}
	demote_env_token(tokens);
}
