/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbarhoun <mbarhoun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 15:13:42 by mbarhoun          #+#    #+#             */
/*   Updated: 2025/06/01 09:35:24 by mbarhoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

bool	mvalloc(char ***commands, int size)
{
	*commands = malloc(sizeof(char *) * (size + 1));
	if (!*commands)
		return (printf(ERR_MEM), 0);
	return (1);
}

int	analyze_word(t_token *tokens)
{
	int		r;
	bool	flag;

	r = 0;
	flag = 1;
	while (tokens && tokens->type_token != PIPE)
	{
		if (tokens->type_token == WORD)
		{
			if (flag)
				r++;
			flag = 1;
		}
		if (tokens->type_token != PIPE && tokens->type_token != WORD)
			flag = 0;
		tokens = tokens->next;
	}
	return (r);
}

int	counter_pipe(t_token *tokens)
{
	int		r;
	bool	flag;

	r = 0;
	flag = 1;
	while (tokens)
	{
		if (tokens->type_token != PIPE && flag)
		{
			r++;
			flag = 0;
		}
		if (tokens->type_token == PIPE)
			flag = 1;
		tokens = tokens->next;
	}
	return (r);
}
