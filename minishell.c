/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelhasso <oelhasso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 18:07:15 by oelhasso          #+#    #+#             */
/*   Updated: 2025/09/29 17:47:32 by oelhasso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./minishell.h"
#include "exec/header.h"
#include "exec/gnl/get_next_line.h"
#include <string.h>

static void	*ft_calloc(size_t count, size_t size)
{
	void	*ptr;

	if (size != 0 && count > 18446744073709551615UL / size)
		return (NULL);
	ptr = malloc(count * size);
	if (!ptr)
		return (NULL);
	return (ptr);
}

static int	to_find(const char c, const char *set)
{
	int	i;

	i = 0;
	while (set[i])
	{
		if (c == set[i])
			return (1);
		i++;
	}
	return (0);
}

static char	*ft_strtrim(char const *s1, char const *set)
{
	char	*str;
	size_t	last;
	size_t	first;
	size_t	i;

	if (!s1)
		return (NULL);
	if (!set || !*set || !*s1)
		return (ft_strdup(s1));
	last = ft_strlen(s1) - 1;
	first = 0;
	while (to_find(s1[first], set) && (int)first < ft_strlen(s1))
		first++;
	while (to_find(s1[last], set) && first < last)
		last--;
	str = ft_calloc ((last - first) + 2, 1);
	if (!str)
		return (NULL);
	i = 0;
	while (first <= last)
		str[i++] = s1[first++];
	str[i] = 0;
	return (str);
}

static char	*read_line(t_env *env)
{
	char	*read_line;
	char	*line;

	read_line = NULL;
	line = NULL;
	if (isatty(fileno(stdin)))
		read_line = readline("minishell-$> ");
	else
	{
		line = get_next_line(fileno(stdin));
		read_line = ft_strtrim(line, "\n");
		free(line);
	}
	if (!read_line)
	{
		clean_env(env);
		if (isatty(fileno(stdin)))
			printf("exit\n");
		return (exit(0), NULL);
	}
	if (read_line[0] == '\0')
		return (read_line);
	add_history(read_line);
	return (read_line);
}

int	main(int ac, char **av, char **ev)
{
	char		*buffer;
	t_cmd		*cmd;
	t_env		*env;
	extern int	rl_catch_signals;

	(void)ac;
	(void)av;
	env = handle_env(ev);
	while (1)
	{
		rl_catch_signals = 0;
		set_signals_main();
		buffer = read_line(env);
		cmd = parcing(buffer, env);
		if (cmd)
			execution(cmd, env, ev);
		// while (1);
	}
}
