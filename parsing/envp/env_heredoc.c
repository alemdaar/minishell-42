/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_heredoc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelhasso <oelhasso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 18:04:24 by oelhasso          #+#    #+#             */
/*   Updated: 2025/09/15 19:55:02 by oelhasso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	set_var_0(int *v1, bool *v2, bool *v3)
{
	*v1 = 0;
	*v2 = 0;
	*v3 = 0;
}

static int	inflate_heredoc(char **content, int pos, t_env *env, bool f_quotes)
{
	t_exp	exp;

	set_var_exp(&exp.len_key, &exp.len_value, &exp.r, pos);
	if (is_special((*content)[exp.r], f_quotes))
		return (expand_meta(content, pos, exp.r, f_quotes));
	while ((*content)[exp.r] && is_valid_key((*content)[exp.r]))
		increment(&exp.r, &exp.len_key);
	if (!exp.len_key)
		return (1);
	exp.key = cdup(exp.len_key, *content + (pos + 1));
	exp.value = env_value(exp.key, env);
	if (!exp.value && (*content)[pos + 1] == '\'' && f_quotes)
		return (free_string(&exp.key), 1);
	if (!exp.value)
		*content = key_not_found(content, pos, exp.len_key);
	else
		*content = key_value(content, exp.value, pos, exp.len_key + 1);
	free(exp.key);
	if (exp.value)
	{
		exp.len_value = ft_strlen(exp.value);
		free(exp.value);
	}
	return (exp.len_value);
}

void	resolve_heredoc(t_env *env, char **input, int fd)
{
	bool	d_quotes;
	bool	s_quotes;
	int		r;

	set_var_0(&r, &d_quotes, &s_quotes);
	while ((*input)[r] != '\0')
	{
		if ((*input)[r] == '"' && d_quotes)
			d_quotes = 0;
		else if ((*input)[r] == '\'' && s_quotes)
			s_quotes = 0;
		else if ((*input)[r] == '"' && !s_quotes)
			d_quotes = 1;
		else if ((*input)[r] == '\'' && !d_quotes)
			s_quotes = 1;
		else if ((*input)[r] == '$')
		{
			r += inflate_heredoc(input, r, env, d_quotes);
			continue ;
		}
		r++;
	}
	write(fd, *input, ft_strlen(*input));
}
