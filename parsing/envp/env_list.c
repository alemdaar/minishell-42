/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelhasso <oelhasso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 18:04:35 by oelhasso          #+#    #+#             */
/*   Updated: 2025/09/25 17:54:53 by oelhasso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	env_count(char **env)
{
	int	r;

	r = 0;
	while (env[r])
		r++;
	return (r);
}

static char	*extract_key(char *env)
{
	char	*key;
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (env[i] && env[i] != '=')
		i++;
	key = malloc(i + 1);
	if (!key)
		return (fprintf(stderr, "Allocation Faield\n"), NULL);
	i = 0;
	while (env[j] && env[j] != '=')
		key[i++] = env[j++];
	key[i] = '\0';
	return (key);
}

static char	*extract_value(char *env)
{
	char	*value;
	int		r;
	int		len;

	r = 0;
	len = 0;
	while (env[r] && env[r] != '=')
		r++;
	r++;
	while (env[r + len])
		len++;
	value = malloc(len + 1);
	if (!value)
		return (fprintf(stderr, "Allocation Faield\n"), NULL);
	len = 0;
	while (env[r])
		value[len++] = env[r++];
	value[len] = '\0';
	return (value);
}

static t_env	*create_nenv(char *key, char *value)
{
	t_env	*env;

	env = malloc(sizeof(t_env));
	if (!env)
		return (fprintf(stderr, "Allocation Faield\n"), NULL);
	env->key = key;
	env->value = value;
	env->next = NULL;
	return (env);
}

t_env	*handle_env(char **ev)
{
	t_env	*env;
	t_env	*new;
	t_env	*tmp;
	int		r;
	int		len;

	if (!ev || !*ev)
		return (NULL);
	env = create_nenv(extract_key(ev[0]), extract_value(ev[0]));
	if (!env)
		return (NULL);
	tmp = env;
	len = env_count(ev);
	r = 0;
	while (++r < len)
	{
		new = create_nenv(extract_key(ev[r]), extract_value(ev[r]));
		if (!new)
			return (clean_env(env), fprintf(stderr, "malloc: \n"), NULL);
		tmp->next = new;
		tmp = new;
	}
	return (env);
}
