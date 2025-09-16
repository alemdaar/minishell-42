/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_split.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelhasso <oelhasso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 18:04:40 by oelhasso          #+#    #+#             */
/*   Updated: 2025/09/16 16:52:57 by oelhasso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static bool	is_red(int r, char *content)
{
	if (r-- < 1)
		return (0);
	while (r > 0 && content[r])
	{
		if (content[r] == ' ')
			r -= 1;
		else
			break ;
	}
	if (content[r] == '<' || content[r] == '>')
		return (1);
	return (0);
}

static bool	search_space(char *content)
{
	int	r;

	r = -1;
	while (content[++r])
		if (is_space(content[r]))
			return (1);
	return (0);
}

static int	expand_var1(char **content, int pos, t_env *env)
{
	t_exp	exp;

	set_var_exp(&exp.len_key, &exp.len_value, &exp.r, pos);
	while ((*content)[exp.r] && is_valid_key((*content)[exp.r]))
	{
		exp.r++;
		exp.len_key++;
	}
	exp.key = cdup(exp.len_key, *content + (pos + 1));
	exp.value = env_value(exp.key, env);
	free(exp.key);
	if (!exp.value)
		return (1);
	if (!search_space(exp.value))
		return (free(exp.value), 1);
	exp.value = new_value_quotes(exp.value);
	*content = key_value(content, exp.value, pos, exp.len_key + 1);
	exp.len_value = ft_strlen(exp.value);
	free(exp.value);
	return (exp.len_value);
}

static void	value_quotes(char *content, bool *d_quotes, bool *s_quotes, int r)
{
	if (content[r] == '"' && *d_quotes)
		*d_quotes = 0;
	else if (content[r] == '\'' && *s_quotes)
		*s_quotes = 0;
	else if (content[r] == '"' && !*s_quotes)
		*d_quotes = 1;
	else if (content[r] == '\'' && !*d_quotes)
		*s_quotes = 1;
}

void	env_space(char **input, t_env *env)
{
	bool	d_quotes;
	bool	s_quotes;
	int		r;

	d_quotes = 0;
	s_quotes = 0;
	r = 0;
	while ((*input)[r])
	{
		value_quotes(*input, &d_quotes, &s_quotes, r);
		if ((*input)[r] == '$' && !s_quotes && !d_quotes)
		{
			if (is_red(r, *input))
				break ;
			r += expand_var1(input, r, env);
			continue ;
		}
		r++;
	}
}
