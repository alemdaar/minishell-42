/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelhasso <oelhasso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 00:17:01 by mbarhoun          #+#    #+#             */
/*   Updated: 2025/07/09 12:11:32 by oelhasso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	sigint_handler(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_on_new_line();
	// rl_replace_line("", 0);
	rl_redisplay();
	exit_status(1);
}

static void	sigint_heredoc(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	exit(130);
}

void	set_signals_main(void)
{
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}

void	set_signals_child(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

void	set_signals_heredoc(void)
{
	signal(SIGINT, sigint_heredoc);
}
