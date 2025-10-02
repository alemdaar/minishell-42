/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelhasso <oelhasso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 21:07:34 by oelhasso          #+#    #+#             */
/*   Updated: 2025/10/02 21:36:40 by oelhasso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
#include "../header.h"

static int	check_unset(char *opt)
{
	int	i;

	if (!opt || !opt[0])
		return (print_err("unset", opt, UN_NVI), 1);
	if (!ft_isalpha(opt[0]) && opt[0] != '_')
		return (print_err("unset", opt, UN_NVI), 1);
	i = 1;
	while (opt[i])
	{
		if (!ft_isalnum(opt[i]) && opt[i] != '_')
			return (print_err("unset", opt, UN_NVI), 1);
		i++;
	}
	return (0);
}

static void	remove_env_node(t_env **head, char *key)
{
	t_env	*cur;
	t_env	*prev;

	cur = *head;
	prev = NULL;
	while (cur)
	{
		if (ft_strcmp(cur->key, key) == 0)
		{
			if (prev)
				prev->next = cur->next;
			else
				*head = cur->next;
			free(cur->key);
			free(cur->value);
			free(cur);
			return ;
		}
		prev = cur;
		cur = cur->next;
	}
}

int	builtin_unset(t_cmd *tmp, t_other *other)
{
	int	i;
	int	status;

	status = 0;
	i = 1;
	while (tmp->commands[i])
	{
		if (check_unset(tmp->commands[i]) == 0)
			remove_env_node(&other->envrp, tmp->commands[i]);
		else
			status = 1;
		i++;
	}
	exit_status(status);
	return (status);
}
