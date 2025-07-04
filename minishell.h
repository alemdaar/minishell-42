/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelhasso <oelhasso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 13:19:12 by mbarhoun          #+#    #+#             */
/*   Updated: 2025/07/04 20:18:58 by oelhasso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdlib.h>
# include <stdio.h>
# include <stdbool.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "./parsing/include/token.h"
# include "./parsing/include/syntax.h"
# include "./libft/libft.h"

typedef struct s_env	t_env;
typedef struct s_cmd	t_cmd;
typedef struct s_red	t_red;

typedef struct s_env
{
	char	*key;
	char	*value;
	t_env	*next;
}	t_env;

typedef struct s_red
{
	char	*file;
	// int		open;
	char	*limiter;
	int		pipedoc[2];
	bool	expand;
	t_type	red_type;
	t_red	*next;
}	t_red;

typedef struct s_cmd
{
	char	**commands;
	pid_t	pid;
	int		pipefd[2];
	int		is_limiter;
	int		open1;
	int		open2;
	char	*path_cmd;
	char	*opt;
	char	**argument;
	int		ar;
	t_red	*red;
	int		free_flag;
	t_cmd	*prev;
	t_cmd	*next;
}	t_cmd;

typedef struct s_global
{
	int	exit_status;
}	t_global;

#endif