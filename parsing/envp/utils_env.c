/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelhasso <oelhasso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 18:04:49 by oelhasso          #+#    #+#             */
/*   Updated: 2025/09/16 16:50:01 by oelhasso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

bool	is_valid_key(char c)
{
	return ((ft_isalnum(c) || c == '_') && c != '$');
}

void	set_var_exp(int *v1, int *v2, int *v3, int v4)
{
	*v1 = 0; 
	*v2 = 0;
	*v3 = v4 + 1;
}

void	increment(int *v1, int *v2)
{
	*v1 += 1;
	*v2 += 1;
}

char	*cdup(int size, char *content)
{
	char	*dup;
	int		r;

	r = -1;
	dup = malloc(sizeof(char) * (size + 1));
	if (!dup)
		return (eprintf(ERR_MEM), NULL);
	while (content[++r] && r < size)
		dup[r] = content[r];
	dup[r] = '\0';
	return (dup);
}
