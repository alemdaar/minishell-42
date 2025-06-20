/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   header.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelhasso <oelhasso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 16:56:54 by oelhasso          #+#    #+#             */
/*   Updated: 2025/06/20 22:13:10 by oelhasso         ###   ########.fr       */
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
# include "get_next_line/get_next_line.h"
#include "../minishell.h"

typedef struct s_pipex {
	char			*command;
	char			*path_command;
	char			*opt;
	char			**argument;
	int				ar;
	pid_t			pid;
	int				pipefd[2];
	struct s_pipex	*next;
}	t_pipex;

typedef struct s_other {
	int		count_path;
	char	*all_path;
	char	**paths;
	int		open1;
	int		open2;
	int		prev_read;
	int		is_limiter;
	int		a_pipe;
	int		count_proc;

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

int		exec (t_cmd *cmd, t_env *env);
void	myputstr(char *str, int fd);
void	why_exit(char *str, int type);
char	*mixem(t_pipex *command, t_other *other, int path_ind);
int		mystrlen(char *str);
void	protect_it(t_pipex *command);
int		parcing(t_pipex **command, char **av, t_other *other);
void	edit_paths(t_other *other, char **envp);
int		find_path(t_other *other, char **envp);
void	count_path(t_other *other);
void	fill_path(t_other *other, t_ind *ind);
void	free_all(t_pipex **command, t_other *other);
int		fill_cmd(char *input, t_pipex *command, t_ind *rtn);
int		fill_opt(char *input, t_pipex *command, int pos);
int		fill_opt2(char *input, t_pipex *command, t_ind *ind);
int		check_cmd(t_pipex *command, t_other *other);
int		check_access(t_pipex *command, t_other *other, int path_ind);
int		edit_cmd(t_pipex *command, char **av, int flag);
int		check_file(t_pipex *tmp, t_pipex *command, t_other *other, int flag);
int		fill_argument(t_pipex **command);
int		fill_argument2(t_pipex *tmp, t_ind *ind);
t_pipex	*ft_lstnew(void);
void	ft_lstadd_back(t_pipex **lst, t_pipex *new);
t_pipex	*ft_lstlast(t_pipex *lst);
int		is_awk(char *command);
int		awk_arg(t_pipex **command);
int		awk_arg2(t_pipex *tmp, t_ind *ind);
int		awk_arg3(t_pipex *tmp, t_ind *ind);
int		count_awk_opt(char *opt);
int		execution(t_pipex *command, t_other *other);
int		close_fds(int fds[2], int file);
int		make_heredoc(t_other *other);
int		is_limiter(char *line, char *limiter);
int		dupping2(t_pipex *tmp, t_other *other, int position);
int		execution2(t_pipex *tmp, t_pipex *command, t_other *other, int i);
int		set_parcing(t_other *other);
int		check_file2(t_pipex *command, t_other *other, int flag);
int		child_process(t_pipex *tmp, t_pipex *command, t_other *other, int position);
#endif