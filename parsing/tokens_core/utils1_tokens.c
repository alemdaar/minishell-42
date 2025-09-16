/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1_tokens.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelhasso <oelhasso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 18:06:27 by oelhasso          #+#    #+#             */
/*   Updated: 2025/09/16 12:23:29 by oelhasso         ###   ########.fr       */
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

bool	check_quotes(char *input)
{
	int		r;
	bool	sq;
	bool	dq;

	r = 0;
	sq = 0;
	dq = 0;
	while (input[r])
	{
		if (!dq && input[r] == '\'')
			sq = !sq;
		if (!sq && input[r] == '"')
			dq = !dq;
		r++;
	}
	return (!sq && !dq);
}
