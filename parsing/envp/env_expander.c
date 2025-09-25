/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_expander.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelhasso <oelhasso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 18:04:19 by oelhasso          #+#    #+#             */
/*   Updated: 2025/09/25 21:33:38 by oelhasso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*env_value(char *key, t_env *env)
{
	t_env	*tmp;
	char	*value;

	tmp = env;
	value = NULL;
	while (tmp)
	{
		if (!ft_strcmp(tmp->key, key))
			break ;
		tmp = tmp->next;
	}
	if (tmp)
		value = ft_strdup(tmp->value);
	return (value);
}

char	*key_not_found(char **content, int pos_key, int len_key)
{
	char	*value;
	int		r;
	int		f;

	r = -1;
	f = 0;
	value = malloc(sizeof(char) * (ft_strlen(*content) - len_key + 2));
	if (!value)
		return (eprintf(ERR_MEM), NULL);
	while ((*content)[++r])
	{
		if (r == pos_key)
		{
			r += len_key;
			continue ;
		}
		value[f++] = (*content)[r];
	}
	value[f] = '\0';
	free(*content);
	return (value);
}

char	*key_value(char **content, char *v_env, int pos, int len_key)
{
	char	*value;
	int		r;
	int		g;
	int		f;

	r = -1;
	g = -1;
	f = 0;
	value = malloc(ft_strlen(*content) - len_key + ft_strlen(v_env) + 1);
	if (!value)
		return (eprintf(ERR_MEM), NULL);
	while ((*content)[++r] && r < (int)ft_strlen(*content))
	{
		if (r == pos)
		{
			while (v_env[++g])
				value[f++] = v_env[g];
			r += len_key - 1;
			continue ;
		}
		value[f++] = (*content)[r];
	}
	value[f] = '\0';
	free(*content);
	return (value);
}

static int	expand_var(t_token **t, t_ambg amb, t_env *env, bool f_quotes)
{
	t_exp	exp;

	set_var_exp(&exp.len_key, &exp.len_value, &exp.r, amb.r);
	exp.len_key = 0; 
	exp.len_value = 0;
	exp.r = amb.r + 1;
	if (is_special((*t)->content[exp.r], f_quotes))
		return (expand_meta(&(*t)->content, amb.r, exp.r, f_quotes));
	while ((*t)->content[exp.r] && is_valid_key((*t)->content[exp.r]))
		incre(&exp.r, &exp.len_key);
	exp.key = cdup(exp.len_key, (*t)->content + (amb.r + 1));
	exp.value = env_value(exp.key, env);
	if (!exp.value && (*t)->content[amb.r + 1] == '\'' && f_quotes)
		return (free_string(&exp.key), 1);
	if (!exp.value)
		(*t)->content = no_exp_val(t, amb, exp);
	else
		set_new_content(t, &exp, &amb);
	free(exp.key);
	if (exp.value)
		valid_exp_key(&exp);
	if ((*t)->amb == 1)
		return (-3);
	return (exp.len_value);
}

void	is_env(t_token **token, t_env *env, bool expander, bool ambg)
{
	t_ambg	amb;
	int		r;

	init_amb(&amb, ambg);
	while ((*token)->content[amb.r])
	{
		if ((*token)->content[amb.r] == '"' && amb.d_quotes)
			amb.d_quotes = 0;
		else if ((*token)->content[amb.r] == '\'' && amb.s_quotes)
			amb.s_quotes = 0;
		else if ((*token)->content[amb.r] == '"' && !amb.s_quotes)
			amb.d_quotes = 1;
		else if ((*token)->content[amb.r] == '\'' && !amb.d_quotes)
			amb.s_quotes = 1;
		else if ((*token)->content[amb.r] == '$' && !amb.s_quotes && expander)
		{
			r = expand_var(token, amb, env, amb.d_quotes);
			if (r == -3)
				return ;
			amb.r += r;
			continue ;
		}
		amb.r++;
	}
}
