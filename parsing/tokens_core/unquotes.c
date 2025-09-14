/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unquotes.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelhasso <oelhasso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 18:06:20 by oelhasso          #+#    #+#             */
/*   Updated: 2025/09/14 18:33:32 by oelhasso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

bool	has_quotes(char *str)
{
	int	r;

	r = -1;
	while (str[++r])
		if (str[r] == '\'' || str[r] == '"')
			return (1);
	return (0);
}

static int	end_quotes(char *str, char c)
{
	int	r;

	r = 0;
	while (str[r] && str[r] != c)
		r++;
	return (r);
}

static int	analyze_quotes(char *str)
{
	int	r;
	int	all_quotes;
	int	quotes;

	r = 0;
	quotes = 0;
	all_quotes = 0;
	while (str[r] && r < (int)ft_strlen(str))
	{
		if (str[r] == '"' || str[r] == '\'')
		{
			quotes += 2;
			r += end_quotes(str + r + 1, str[r]) + 1;
		}
		r++;
	}
	r = 0;
	while (str[r])
	{
		if (str[r] == '"' || str[r] == '\'')
			all_quotes++;
		r++;
	}
	return (all_quotes - quotes);
}

static int	skip_quotes_len(char *str)
{
	int	len;
	int	r;

	len = 0;
	r = 0;
	while (str[r])
	{
		if (str[r] != '"' && str[r] != '\'')
			len++;
		r++;
	}
	return (len);
}

char	*remove_quotes(char *str)
{
	t_refactor	t_var;

	t_var.r = 0;
	t_var.f = 0;
	t_var.flag = 0;
	t_var.new = malloc((skip_quotes_len(str) + analyze_quotes(str) + 1));
	if (!t_var.new)
		return (p1char(&str), eprintf(ERR_MEM), NULL);
	while (str[t_var.r])
	{
		if (!t_var.flag && (str[t_var.r] == '\'' || str[t_var.r] == '"'))
		{
			t_var.quotes = str[t_var.r];
			t_var.flag = 1;
		}
		else if (str[t_var.r] != t_var.quotes)
			t_var.new[t_var.f++] = str[t_var.r];
		else
			t_var.flag = 0;
		t_var.r++;
	}
	p1char(&str);
	t_var.new[t_var.f] = '\0';
	return (t_var.new);
}
