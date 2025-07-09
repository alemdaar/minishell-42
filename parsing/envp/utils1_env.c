/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbarhoun <mbarhoun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 20:26:09 by mbarhoun          #+#    #+#             */
/*   Updated: 2025/07/08 09:23:34 by mbarhoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	err_wrt(const char *file)
{
	write(2, "minishell: $", 12);
	write(2, file, ft_strlen(file));
	write(2, ": ambiguous redirect\n", 21);
}

char	*new_value_quotes(char *str)
{
	int		r;
	int		r1;
	char	strr[70000];

	r = -1;
	r1 = 0;
	while (str[++r])
	{
		if (str[r] == '"')
		{
			strr[r1++] = '\'';
			strr[r1++] = str[r];
			strr[r1++] = '\'';
		}
		if (str[r] == '\'')
		{
			strr[r1++] = '"';
			strr[r1++] = str[r];
			strr[r1++] = '"';
		}
		if (str[r] != '\'' && str[r] != '"')
			strr[r1++] = str[r];
	}
	strr[r1] = '\0';
	return (free(str), ft_strdup(strr));
}

void	set_new_content(t_token **token, t_exp *exp, t_ambg *amb)
{
	if (count_word(exp->value) > 1 && amb->ambiguous)
	{
		(*token)->amb = 1;
		err_wrt(exp->key);
		(*token)->content = key_not_found(&(*token)->content, amb->r, \
							exp->len_key);
		return ;
	}
	(*token)->content = key_value(&(*token)->content, exp->value, \
						amb->r, exp->len_key + 1);
}
