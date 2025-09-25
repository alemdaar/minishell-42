/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extra.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelhasso <oelhasso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 15:54:03 by oelhasso          #+#    #+#             */
/*   Updated: 2025/09/25 20:42:38 by oelhasso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	incre(int *a, int *b)
{
	a ++;
	b ++;
	return ;
}

char	*no_exp_val(t_token **t, t_ambg amb, t_exp exp)
{
	ambiguous_redirect(t, amb.ambiguous, exp.key);
	(*t)->content = key_not_found(&(*t)->content, amb.r, exp.len_key);
	return ((*t)->content);
}

void	valid_exp_key(t_exp *exp)
{
	exp->len_value = ft_strlen(exp->value);
	free(exp->value);
}

void	init_amb(t_ambg *amb, bool ambg)
{
	printf ("INIT\n");
	amb->d_quotes = 0;
	amb->s_quotes = 0;
	amb->ambiguous = ambg;
	amb->r = 0;
	printf ("INIT FINISHED\n");
	return ;
}
