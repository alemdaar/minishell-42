/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_scrap.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelhasso <oelhasso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 18:05:54 by oelhasso          #+#    #+#             */
/*   Updated: 2025/09/14 18:05:55 by oelhasso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	err_wrt(const char *red)
{
	write(2, "minishell: syntax error near unexpected token `", 47);
	write(2, red, ft_strlen(red));
	write(2, "'\n", 2);
}

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
	err_wrt(red);
}

void	is_vred(t_token *tokens)
{
	while (tokens)
	{
		if (tokens->type_token != WORD && tokens->type_token != PIPE
			&& tokens->type_token != ENV)
			tokens->red = 1;
		tokens = tokens->next;
	}
}
