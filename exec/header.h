/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   header.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelhasso <oelhasso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 16:56:54 by oelhasso          #+#    #+#             */
/*   Updated: 2025/09/29 10:53:48 by oelhasso         ###   ########.fr       */
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
# include <errno.h>
# include <sys/stat.h>
# include "../minishell.h"

// struct stat st;
// extern struct stat st;

typedef struct s_other {
	int			count_path;
	char		*all_path;
	char		**paths;
	int			a_pipe;
	int			count_proc;
	char		**envr;
	t_env		*envrp;
	t_env		*old_pwd;
	int			exit_status;
	int			stdout_copy;
	int			stdout_flag;
	int			stdin_copy;
	int			stdin_flag;
	t_cmd		*orig_cmd;
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

int		execution(t_cmd *cmd, t_env *env, char **ev);
void	ft_lstadd_back(t_env **lst, t_env *new);
t_env	*ft_lstlast(t_env *lst);
t_env	*ft_lstnew(char *content1, char *content2);
int		run_bin(t_cmd *tmp, t_other *other);
int		builtin_cd(t_cmd *cmd, t_other *other);
int		dupping(t_cmd *tmp, t_other *other);
int		child_process(t_cmd *tmp, t_other *other, int position);
int		fill_argument(t_cmd *tmp, t_other *other);
int		check_cmd(t_cmd *cmd, t_other *other);
int		builtin_echo(t_cmd *tmp);
int		builtin_env(t_cmd *tmp, t_other *other);
int		exec(t_cmd *tmp, t_other *other);
int		execution2(t_cmd *tmp, t_other *other, int i);
void	handle_exit_status(int status);
int		exit_status(int val);
int		builtin_exit(t_cmd *tmp);
int		builtin_export(t_cmd *tmp, t_other *other);
int		file_pipe(t_cmd *tmp, int flag);
int		check_file(t_cmd *tmp, t_other *other, int flag);
void	free_env(t_other *other);
int		free_all(t_other *other);
int		child_doc(t_cmd *tmp, t_other *other, t_ind *ind);
int		edit_paths(t_other *other, t_env *env);
int		builtin_pwd(t_other *other);
int		is_builtin(t_cmd *tmp);
int		count_heredoc(t_cmd *tmp);
void	count_args(t_cmd *tmp);
int		set_up(t_cmd *tmp);
int		pipping(t_cmd *tmp, int type);
void	close_set(int *to_be_closed, int value);
void	print_err(char *cmnd, char *sub, char *msg);
int		is_limiter(char *line, char *limiter);
int		is_equal(char *command, char *b_in);
size_t	mystrlen(char *str);
void	why_exit(char *str, int flag);
int		builtin_unset(t_cmd *tmp, t_other *other);
int		work(t_cmd *cmd, t_other *other);
int		restore_fds(t_other *other);
void	close_fds(t_cmd *tmp);
void	close_all_fds(t_cmd *cmd);

#endif