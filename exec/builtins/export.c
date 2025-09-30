/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelhasso <oelhasso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 21:19:56 by oelhasso          #+#    #+#             */
/*   Updated: 2025/09/30 22:43:09 by oelhasso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
#include "../header.h"

static int	listenv(t_env *en)
{
	while (en)
	{
		if (en->value)
			printf("declare -x %s=\"%s\"\n", en->key, en->value);
		else
			printf("declare -x %s\n", en->key);
		en = en->next;
	}
	return (0);
}

static int	check_export(char *opt)
{
	int	i;

	if (!opt || !opt[0])
	{
		print_err("export", opt, UN_NVI, 1);
		return (1);
	}
	if (!ft_isalpha(opt[0]) && opt[0] != '_')
	{
		print_err("export", opt, UN_NVI, 1);
		return (1);
	}
	i = 1;
	while (opt[i] && opt[i] != '=')
	{
		if (!ft_isalnum(opt[i]) && opt[i] != '_')
		{
			print_err("export", opt, UN_NVI, 1);
			return (1);
		}
		i++;
	}
	return (0);
}

static int	init_export2(char *key, char *value, t_other *other)
{
	t_env	*new;
	t_env	*cur;

	cur = other->envrp;
	while (cur)
	{
		if (ft_strcmp(cur->key, key) == 0)
		{
			if (value)
			{
				free(cur->value);
				cur->value = value;
			}
			return (free(key), 0);
		}
		cur = cur->next;
	}
	new = ft_lstnew(key, value);
	if (!new)
		return (perror("malloc "), free(key), free(value), 1);
	return (ft_lstadd_back(&other->envrp, new), 0);
}

static int	init_export(char *opt, t_other *other)
{
	char	*key;
	char	*value;
	int		i;
	int		r;

	i = 0;
	while (opt[i] && opt[i] != '=')
		i++;
	key = ft_substr(opt, 0, i);
	if (!key)
		return (perror("malloc "), 1);
	value = NULL;
	if (opt[i] == '=')
	{
		value = ft_strdup(opt + i + 1);
		if (!value)
			return (perror("malloc "), free(key), 1);
	}
	r = init_export2(key, value, other);
	return (r);
}

int	builtin_export(t_cmd *tmp, t_other *other)
{
	int	r;
	int	i;
	int	ret;

	if (!tmp->commands[1])
		return (listenv(other->envrp), 0);
	ret = 0;
	i = 1;
	while (tmp->commands[i])
	{
		r = check_export(tmp->commands[i]);
		if (r == 0)
		{
			if (init_export(tmp->commands[i], other) != 0)
				ret = 1;
		}
		else
			ret = 1;
		i++;
	}
	exit_status(ret);
	return (ret);
}
