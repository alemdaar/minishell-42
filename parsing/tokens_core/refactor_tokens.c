/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   refactor_tokens.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelhasso <oelhasso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 18:06:16 by oelhasso          #+#    #+#             */
/*   Updated: 2025/09/25 21:34:05 by oelhasso         ###   ########.fr       */
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

static bool	is_ambiguous(t_token *token)
{
	if (token->type_token == REDIR_IN || token->type_token == REDIR_OUT
		|| token->type_token == APPEND)
		return (1);
	return (0);
}

void	refactor_tokens(t_token **tokens, t_env *env)
{
	t_token	*token;
	bool	expander;
	bool	ambg;

	token = *tokens;
	expander = 1;
	ambg = 0;
	while (token)
	{
		is_env(&token, env, expander, ambg);
		if (has_quotes(token->content))
		{
			if (!expander)
				token->exp = 0;
			token->content = remove_quotes(token->content);
		}
		expander = 1;
		if (token->type_token == HERDOOC)
			expander = 0;
		ambg = is_ambiguous(token);
		token = token->next;
	}
	demote_env_token(tokens);
}
