/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelhasso <oelhasso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 13:19:12 by mbarhoun          #+#    #+#             */
/*   Updated: 2025/06/20 22:13:16 by oelhasso         ###   ########.fr       */
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
	bool	expand;
	t_type	red_type;
	t_red	*next;
}	t_red;

typedef struct s_cmd
{
	char	**commands;
	t_red	*red;
	char	*infile;
	char	*outfile;
	char	*limiter;
	t_cmd	*prev;
	t_cmd	*next;
}	t_cmd;

typedef struct s_global
{
	int	exit_status;
}	t_global;

#endif