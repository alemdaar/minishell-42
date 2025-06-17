/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3_tokens.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbarhoun <mbarhoun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 14:50:50 by mbarhoun          #+#    #+#             */
/*   Updated: 2025/06/01 15:16:26 by mbarhoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static bool	is_symbols_space(char c)
{
	if (is_symbols(c) || is_space(c))
		return (1);
	return (0);
}

void	cut_value_quotes(int *r, char *tokens, char c, bool flag)
{
	*r = 1;
	while (tokens[*r])
	{
		if (tokens[*r] == c)
		{
			flag = 0;
			*r = *r + 1;
		}
		if (!flag && tokens[*r] && is_symbols_space(tokens[*r]))
			break ;
		if (!flag && (tokens[*r] == '\'' || tokens[*r] == '"') && tokens[*r])
		{
			flag = 1;
			c = tokens[*r];
			*r = *r + 1;
		}
		if (tokens[*r] && tokens[*r] != c)
			*r = *r + 1;
	}
}
