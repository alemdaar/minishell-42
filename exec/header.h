/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   header.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelhasso <oelhasso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 16:56:54 by oelhasso          #+#    #+#             */
/*   Updated: 2025/06/30 17:54:56 by oelhasso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEADER_H
# define HEADER_H

# define SUCCESSFUL 0
# define FAILED 1
# define TRUE 1
# define FALSE 0
# define ERROR -1
# define INPUT 0
# define DEBUG 1
# define OUTPUT 1
# define READ 0
# define WRITE 1
# define STD_OUT 1
# define STD_ERR 2

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>
# include <sys/wait.h>
#include "../minishell.h"

// typedef struct s_cmd {
// 	char			*command;
// 	char			*path_command;
// 	char			*opt;
// 	char			**argument;
// 	int				ar;
// 	struct s_cmd	*next;
// }	t_cmd;

typedef struct s_other {
	int		count_path;
	char	*all_path;
	char	**paths;
	int		prev_read;
	int		a_pipe;
	int		count_proc;
	char	**envr;
	t_cmd	*orig_cmd;
}	t_other;

typedef struct s_ind
{
	int	i;
	int	j;
	int	c;
	int	r;
	int	t;
	int	f;
}	t_ind;

int		execution(t_cmd *cmd, t_env *env);
// void	myputstr(char *str, int fd);
// void	why_exit(char *str, int type);
// char	*mixem(t_cmd *command, t_other *other, int path_ind);
// int		mystrlen(char *str);
// void	protect_it(t_cmd *command);
// int		parcing(t_cmd **command, char **av, t_other *other);
// void	edit_paths(t_other *other, char **envp);
// int		find_path(t_other *other, char **envp);
// void	count_path(t_other *other);
// void	fill_path(t_other *other, t_ind *ind);
// void	free_all(t_cmd **command, t_other *other);
// int		fill_cmd(char *input, t_cmd *command, t_ind *rtn);
// int		fill_opt(char *input, t_cmd *command, int pos);
// int		fill_opt2(char *input, t_cmd *command, t_ind *ind);
// int		check_cmd(t_cmd *command, t_other *other);
// int		check_access(t_cmd *command, t_other *other, int path_ind);
// int		edit_cmd(t_cmd *command, char **av, int flag);
// int		check_file(t_cmd *tmp, t_cmd *command, t_other *other, int flag);
// int		fill_argument(t_cmd **command);
// int		fill_argument2(t_cmd *tmp, t_ind *ind);
// t_cmd	*ft_lstnew(void);
// void	ft_lstadd_back(t_cmd **lst, t_cmd *new);
// t_cmd	*ft_lstlast(t_cmd *lst);
// int		is_awk(char *command);
// int		awk_arg(t_cmd **command);
// int		awk_arg2(t_cmd *tmp, t_ind *ind);
// int		awk_arg3(t_cmd *tmp, t_ind *ind);
// int		count_awk_opt(char *opt);
// int		execution(t_cmd *command, t_other *other);
// int		close_fds(int fds[2], int file);
// int		make_heredoc(t_other *other);
// int		is_limiter(char *line, char *limiter);
// int		dupping2(t_cmd *tmp, t_other *other, int position);
// int		execution2(t_cmd *tmp, t_cmd *command, t_other *other, int i);
// int		set_parcing(t_other *other);
// int		check_file2(t_cmd *command, t_other *other, int flag);
// int		child_process(t_cmd *tmp, t_cmd *command, t_other *other, int position);
#endif