/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelhasso <oelhasso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 20:17:36 by mbarhoun          #+#    #+#             */
/*   Updated: 2025/06/29 18:22:36 by oelhasso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static bool	is_spaces(char *input)
{
	int	r;
	int	spc;
	int	tmp;

	r = -1;
	spc = 0;
	while (input[++r])
		if (is_space(input[r]))
			spc++;
	tmp = ft_strlen(input);
	if (spc == tmp)
		return (1);
	return (0);
}

void	glance_input(char *input, t_env *env)
{
	if (!input)
		exit(0);
	if (!ft_strcmp(input, "exit"))
	{
		env_leaks(env);
		printf("exit\n");
		exit(0);
	}
	if (input[0] == '\0' || is_spaces(input))
		return ;
	add_history(input);
}
