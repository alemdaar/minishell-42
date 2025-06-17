/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_leaks.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbarhoun <mbarhoun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 17:07:41 by mbarhoun          #+#    #+#             */
/*   Updated: 2025/06/01 09:34:57 by mbarhoun         ###   ########.fr       */
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
