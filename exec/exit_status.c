/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_status.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelhasso <oelhasso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 15:07:49 by oelhasso          #+#    #+#             */
/*   Updated: 2025/09/25 15:06:54 by oelhasso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "./header.h"

int	exit_status(int val)
{
	static int	exit;

	if (val > -1)
		exit = val;
	return (exit);
}

void	handle_exit_status(int status)
{
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
			exit_status(130);
		else if (WTERMSIG(status) == SIGQUIT)
		{
			exit_status(131);
			write(1, "Quit: 3\n", 8);
		}
	}
	else if (WIFEXITED(status))
		exit_status(WEXITSTATUS(status));
}
