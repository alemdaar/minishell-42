/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_scrap.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbarhoun <mbarhoun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 13:54:17 by mbarhoun          #+#    #+#             */
/*   Updated: 2025/06/01 09:34:36 by mbarhoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	wr_syntax(t_type type)
{
	char	*red;

	if (type == HERDOOC)
		red = "<<";
	if (type == APPEND)
		red = ">>";
	if (type == REDIR_IN)
		red = "<";
	if (type == REDIR_OUT)
		red = ">";
	printf("minishell: syntax error near unexpected token `%s'\n", red);
}

void	is_vred(t_token *tokens)
{
	while (tokens)
	{
		if (tokens->type_token != WORD && tokens->type_token != PIPE)
			tokens->red = 1;
		tokens = tokens->next;
	}
}
