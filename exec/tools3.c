/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelhasso <oelhasso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 21:59:10 by oelhasso          #+#    #+#             */
/*   Updated: 2025/09/30 22:46:50 by oelhasso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "./header.h"

void	print_err(char *cmnd, char *sub, char *msg, int flag)
{
	char *shell;

	shell = "minishell: ";
	write (2, "%s", ft_strlen(shell));
	if (cmnd)
		write (2, "%s", ft_strlen(cmnd));
	if (cmnd)
		write (2, ": ", ft_strlen(": "));
	if (sub)
		write (2, "%s", ft_strlen(sub));
	if (sub)
		write (2, ": ", ft_strlen(": "));
	if (msg)
		write (2, "%s", ft_strlen(msg));
	if (flag)
		write (2, "\n", ft_strlen("\n"));
	return ;
}

int	is_limiter(char *line, char *limiter)
{
	t_ind	ind;

	ind.i = 0;
	while (limiter[ind.i] && limiter[ind.i] == line[ind.i])
		ind.i ++;
	if (limiter[ind.i] == 0 && line[ind.i] == '\n')
		return (TRUE);
	return (FALSE);
}

int	is_equal(char *command, char *b_in)
{
	int	i;

	i = 0;
	while (command[i] && b_in[i] && b_in[i] == command[i])
		i++;
	if (b_in[i] == command[i])
		return (1);
	return (0);
}

size_t	mystrlen(char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

void	why_exit(char *str, int flag)
{
	if (flag == SUCCESSFUL)
		perror (str);
	else if (flag == FAILED)
		printf ("%s", str);
	exit_status(flag);
	exit(flag);
}
