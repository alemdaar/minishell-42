/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelhasso <oelhasso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 21:06:44 by oelhasso          #+#    #+#             */
/*   Updated: 2025/09/18 21:07:13 by oelhasso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "./header.h"

int builtin_env(t_cmd *tmp, t_other *other)
{
    t_env *tmpo;

    if (tmp->commands[1])
        return (0);
    tmpo = other->envrp;
    while (tmpo)
    {
        if (tmpo->value)
            printf("%s=%s\n", tmpo->key, tmpo->value);
        tmpo = tmpo->next;
    }
    return (0);
}
