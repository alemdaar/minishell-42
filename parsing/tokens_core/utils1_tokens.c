/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1_tokens.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelhasso <oelhasso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 18:06:27 by oelhasso          #+#    #+#             */
/*   Updated: 2025/09/14 18:06:28 by oelhasso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

bool	has_env(char *content)
{
	int	r;

	r = 0;
	while (content[r])
	{
		if (content[r] == '$')
			return (1);
		r++;
	}
	return (0);
}

bool	is_symbols(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

bool	is_space(char c)
{
	return ((c >= 9 && c <= 13) || c == 32);
}

bool	quotes_is_valid(char *tokens)
{
	int		r;
	bool	s_quotes;
	bool	d_quotes;

	r = -1;
	s_quotes = 0;
	d_quotes = 0;
	while (tokens[++r])
	{
		if (tokens[r] == '\'' && !d_quotes)
			s_quotes = !s_quotes;
		if (tokens[r] == '"' && !s_quotes)
			d_quotes = !d_quotes;
	}
	return (!s_quotes && !d_quotes);
}
