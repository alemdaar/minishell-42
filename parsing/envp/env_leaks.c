/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_leaks.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelhasso <oelhasso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 18:04:28 by oelhasso          #+#    #+#             */
/*   Updated: 2025/09/14 18:04:29 by oelhasso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	env_leaks(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		p1char(&env->key);
		p1char(&env->value);
		tmp = env;
		env = env->next;
		free(tmp);
	}
}
