/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelhasso <oelhasso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 18:07:23 by oelhasso          #+#    #+#             */
/*   Updated: 2025/09/25 20:27:29 by oelhasso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define ERR_QUOTE "Error Quotes\n"
# define ERR_PIPE "minishell: syntax error near unexpected token `|'\n"
# define ERR_RED "minishell: syntax error near unexpected token `newline'\n"
# define NO_DIR "No such file or directory\n"
# define IS_DIR "is a directory\n"
# define NO_PERM "Permission denied\n"
# define EX_NA "numeric argument required\n"
# define EX_TA "too many arguments\n"
# define UN_NVI "not a valid identifier\n"
# define NO_HOME "minishell: cd: HOME not set\n"
# define NO_OLDPWD "minishell: cd: OLDPWD not set\n"

# include <stdlib.h>
# include <stdio.h>
# include <stdbool.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "./parsing/include/token.h"
# include "./libft/libft.h"
# include <limits.h>

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
	bool	amb;
	pid_t	pid;
	int		pipedoc[2];
	int		pipefd[2];
	char	*limiter;
	int		is_limiter;
	int		open1;
	int		open2;
	char	*path_cmd;
	char	**argument;
	int		ar;
	t_red	*red;
	int		free_flag;
	int		bin;
	int		count_doc;
	int		flag_exit;
	t_cmd	*prev;
	t_cmd	*next;
}	t_cmd;

#endif