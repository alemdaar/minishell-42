/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelhasso <oelhasso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 22:37:51 by oelhasso          #+#    #+#             */
/*   Updated: 2025/09/25 13:35:20 by oelhasso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
#include "../header.h"

static void	replace_pwd2(t_env *en, int len)
{
	if (en->value)
	{
		free (en->value);
		en->value = NULL;
	}
	en->value = malloc (len + 1);
	return ;
}

static int	replace_pwd(int len, t_other *other, char *cwdp)
{
	int		i;
	t_env	*en;
	char	*path;

	path = "PWD";
	i = 0;
	en = other->envrp;
	while (en)
	{
		while (path[i] && en->key[i] && path[i] == en->key[i])
			i++;
		if (path[i] == en->key[i] && !path[i] && !en->key[i])
		{
			replace_pwd2(en, len);
			if (!en->value)
				return (perror("malloc: "), -1);
			i = -1;
			while (cwdp[++i])
				en->value[i] = cwdp[i];
			return (en->value[i] = 0, SUCCESSFUL);
		}
		en = en->next;
	}
	return (SUCCESSFUL);
}

int	builtin_pwd(t_other *other)
{
	char	cwd[PATH_MAX];
	char	*cwdp;
	int		len;
	int		r;

	if (getcwd(cwd, sizeof(cwd)) == NULL)
		return (perror("pwd "), 1);
	len = ft_strlen(cwd);
	cwdp = cwd;
	r = replace_pwd(len, other, cwdp);
	if (r == -1)
		return (-1);
	printf("%s\n", cwd);
	return (0);
}
