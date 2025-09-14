/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scrap.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelhasso <oelhasso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 18:05:50 by oelhasso          #+#    #+#             */
/*   Updated: 2025/09/14 18:05:51 by oelhasso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static bool	is_red_pipe(t_token *t1, t_token *t2)
{
	if (t1->red && t2->type_token == PIPE)
		return (eprintf(ERR_PIPE), 1);
	if (t1->red && t2->red)
		return (wr_syntax(t2->type_token), 1);
	if (t1->type_token == PIPE && t2->type_token == PIPE)
		return (eprintf(ERR_PIPE), 1);
	if (t2->type_token == PIPE && !t2->next)
		return (eprintf(ERR_PIPE), 1);
	if (t2->red && !t2->next)
		return (eprintf(ERR_RED), 1);
	return (0);
}

static bool	scrap_search(t_token *tokens)
{
	t_token	*next;

	next = tokens->next;
	while (next)
	{
		if (is_red_pipe(tokens, next))
			return (0);
		next = next->next;
		tokens = tokens->next;
	}
	return (1);
}

bool	all_scrap(t_token *tokens)
{
	is_vred(tokens);
	if (tokens->type_token == PIPE)
		return (eprintf(ERR_PIPE), 0);
	if (tokens->red && !tokens->next)
		return (eprintf(ERR_RED), 0);
	return (scrap_search(tokens));
}
