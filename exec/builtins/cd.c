/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelhasso <oelhasso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 21:55:43 by oelhasso          #+#    #+#             */
/*   Updated: 2025/09/30 22:52:24 by oelhasso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
#include "../header.h"

static char	*extract_value(t_env *env, const char *key)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

static int	update_env2(t_env *env, char *key, char *value)
{
	t_env	*tmp;
	t_env	*new_node;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return (perror("malloc"), -1);
	new_node->key = ft_strdup(key);
	new_node->value = ft_strdup(value);
	new_node->next = NULL;
	if (!new_node->key || !new_node->value)
		return (perror("malloc"), -1);
	tmp = env;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_node;
	return (0);
}

static int	update_env(t_env *env, char *key, char *value)
{
	t_env	*tmp;
	int		r;

	tmp = env;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, key) == 0)
		{
			free(tmp->value);
			tmp->value = ft_strdup(value);
			if (!tmp->value)
				return (perror("malloc"), -1);
			return (0);
		}
		tmp = tmp->next;
	}
	r = update_env2(env, key, value);
	return (r);
}

static char	*builtin_cd2(t_cmd *cmd, t_env *env, char *oldpwd)
{
	char	*target;

	target = NULL;
	if (!cmd->commands[1] 
		|| (cmd->commands[1][0] == '~' && !cmd->commands[1][1]))
	{
		target = extract_value(env, "HOME");
		if (!target)
			return (print_err(NULL, NULL, NO_HOME, 1), free(oldpwd), NULL);
	}
	else if (ft_strcmp(cmd->commands[1], "-") == 0)
	{
		target = extract_value(env, "OLDPWD");
		if (!target)
		{
			print_err(NULL, NULL, NO_OLDPWD, 1);
			free(oldpwd);
			return (NULL);
		}
		printf("%s\n", target);
	}
	else
		target = cmd->commands[1];
	return (target);
}

int	builtin_cd(t_cmd *cmd, t_other *other)
{
	char	*target;
	char	*oldpwd;
	char	cwd[PATH_MAX];
	t_env	*env;

	env = other->envrp;
	if (!getcwd(cwd, sizeof(cwd)))
		return (perror("getcwd"), 1);
	oldpwd = ft_strdup(cwd);
	if (!oldpwd)
		return (perror("malloc"), 1);
	target = builtin_cd2(cmd, env, oldpwd);
	if (target == NULL)
		return (1);
	if (chdir(target) == -1)
		return (perror("minishell: cd"), free(oldpwd), 1);
	if (update_env(env, "OLDPWD", oldpwd) == -1) 
		return (free(oldpwd), 1);
	free(oldpwd);
	if (!getcwd(cwd, sizeof(cwd)))
		return (perror("getcwd"), 1);
	if (update_env(env, "PWD", cwd) == -1)
		return (1);
	return (0);
}
