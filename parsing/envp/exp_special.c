/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_special.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbarhoun <mbarhoun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 13:37:36 by mbarhoun          #+#    #+#             */
/*   Updated: 2025/07/06 18:11:55 by mbarhoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static bool	is_expempty(char c)
{
	if (c == '@' || c == '*')
		return (1);
	return (0);
}

void	ambiguous_redirect(t_token **token, bool amb, char *key)
{
	if (amb)
	{
		printf("minishell: $%s: ambiguous redirect\n", key);
		(*token)->amb = 1;
	}
}

bool	is_special(char c, bool f_quotes)
{
	if ((!ft_isalpha(c) && c != '\'' && c != '"' && c != '_') \
		|| (c == '"' && f_quotes))
		return (1);
	return (0);
}

int	count_word(char *content)
{
	bool	space;
	int		len;
	int		r;

	r = 0;
	len = 0;
	space = 1;
	while (content[r])
	{
		if (space)
		{
			len++;
			space = 0;
		}
		if (is_space(content[r]))
			space = 1;
		r++;
	}
	return (len);
}

int	expand_meta(char **content, int pos, int r, bool f_quotes)
{
	int		ival;
	char	*sval;

	sval = NULL;
	if ((*content)[r] == '\0' || ((*content)[r] == '"' && f_quotes))
		return (1);
	if ((*content)[r] == '0')
		return (*content = key_value(content, "minishell", pos, 2), \
				ft_strlen("minishell"));
	if (ft_isdigit((*content)[r]) || is_expempty((*content)[r]))
		return (*content = key_not_found(content, pos, 1), 0);
	if ((*content)[r] == '?')
	{
		ival = exit_status(-1);
		sval = ft_itoa(ival);
		return (*content = key_value(content, sval, pos, 2), \
				p1char(&sval), ft_strlen("0"));
	}
	return (1);
}
