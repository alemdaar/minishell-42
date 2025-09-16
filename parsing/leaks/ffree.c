/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ffree.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelhasso <oelhasso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 18:05:36 by oelhasso          #+#    #+#             */
/*   Updated: 2025/09/16 10:40:01 by oelhasso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	p2char(char ***ptr)
{
	int	r;

	if (!ptr || !*ptr)
		return ;
	r = 0;
	while ((*ptr)[r])
		free((*ptr)[r++]);
	free(*ptr);
	*ptr = NULL;
}

void	free_string(char **ptr)
{
	if (!*ptr || !ptr)
		return ;
	free(*ptr);
	*ptr = NULL;
}

void	free_parcing(t_token *tokens, char *input)
{
	t_token	*tmp;

	while (tokens)
	{
		tmp = tokens;
		free_string(&tokens->content);
		tokens = tokens->next;
		free(tmp);
	}
	free_string(&input);
}
