/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelhasso <oelhasso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 18:05:20 by oelhasso          #+#    #+#             */
/*   Updated: 2025/09/15 20:27:03 by oelhasso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_H
# define TOKEN_H

# define ERR_MEM "Allocation Faield\n"

typedef struct s_token	t_token;
typedef struct s_env	t_env;
typedef struct s_cmd	t_cmd;
typedef struct s_red	t_red;

typedef enum e_token_type
{
	WORD,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	APPEND,
	HERDOOC,
	ENV,
}	t_type;

typedef struct s_token
{
	char	*content;
	bool	red;
	bool	exp;
	bool	amb;
	t_type	type_token;
	t_token	*next;
}	t_token;

typedef struct s_spl
{
	int		i;
	int		r;
	bool	f_quotes;
	char	*new_token;
}	t_spl;

typedef struct s_refactor
{
	char	*new;
	char	quotes;
	int		r;
	int		f;
	bool	flag;
}	t_refactor;

typedef struct s_exp
{
	int		r;
	int		len_key;
	int		len_value;
	char	*value;
	char	*key;
}	t_exp;

typedef struct s_norm
{
	int	r;
	int	pipe;
}	t_norm;

typedef struct s_cm
{
	int	size;
	int	r;
	int	i;
}	t_cm;

typedef struct s_cm1
{
	t_red	*new_red;
	t_red	*tmp_red;
}	t_cm1;

typedef struct s_amb
{
	bool	d_quotes;
	bool	s_quotes;
	bool	ambiguous;
	int		r;
}	t_ambg;

/*~~~~~~~~~~~~~~~~~~~~~~~<history.c>~~~~~~~~~~~~~~~~~~~~~~~*/
void		history_input(char *input);
/*~~~~~~~~~~~~~~~~~~~~~~~<token.c>~~~~~~~~~~~~~~~~~~~~~~~*/
t_cmd		*assemble_command(char *input, t_env *env);
/*~~~~~~~~~~~~~~~~~~~~<list_tokens.c>~~~~~~~~~~~~~~~~~~~~~*/
t_token		*segment_input(char *tokens);
/*~~~~~~~~~~~~~~~~~~<refactor_tokens.c>~~~~~~~~~~~~~~~~~~~*/
void		refactor_tokens(t_token **tokens, t_env *env);
/*~~~~~~~~~~~~~~~~~~~~~~<unquotes.c>~~~~~~~~~~~~~~~~~~~~~~*/
bool		has_quotes(char *str);
char		*remove_quotes(char *str);
/*~~~~~~~~~~~~~~~~~~~~<utils0_tokens.c>~~~~~~~~~~~~~~~~~~*/
int			skip_space(char *str);
int			add_value(int r, bool *f_quotes);
void		set_var_zero(int *v1, int *v2, bool *v3, t_token **lv4);
void		node_back(t_token **main, t_token *new);
t_token		*create_node(char *content);
/*~~~~~~~~~~~~~~~~~~~~<utils1_tokens.c>~~~~~~~~~~~~~~~~~~*/
bool		has_env(char *content);
bool		is_symbols(char c);
bool		is_space(char c);
bool		quotes_is_valid(char *tokens);
/*~~~~~~~~~~~~~~~~~~~~<utils3_tokens.c>~~~~~~~~~~~~~~~~~~*/
void		cut_value_quotes(int *r, char *tokens, char c, bool flag);
/*~~~~~~~~~~~~~~~~~~~~~<env_expander.c>~~~~~~~~~~~~~~~~~~~*/
void		is_env(t_token **token, t_env *env, bool expander, bool ambg);
char		*key_value(char **content, char *v_env, int pos, int len_key);
char		*key_not_found(char **content, int pos_key, int len_key);
char		*env_value(char *key, t_env *env);
/*~~~~~~~~~~~~~~~~~~~~~<env_heredoc.c>~~~~~~~~~~~~~~~~~~~*/
void		resolve_heredoc(t_env *env, char **input, int fd);
/*~~~~~~~~~~~~~~~~~~~~~~<env_list.c>~~~~~~~~~~~~~~~~~~~~~~*/
t_env		*construct_env(char **env);
/*~~~~~~~~~~~~~~~~~~~~~~<env_split.c>~~~~~~~~~~~~~~~~~~~~~~*/
void		env_space(char **input, t_env *env);
/*~~~~~~~~~~~~~~~~~~~~~~<exp_special.c>~~~~~~~~~~~~~~~~~~~~~~*/
void		ambiguous_redirect(t_token **token, bool amb, char *key);
bool		is_special(char c, bool f_quotes);
int			count_word(char *content);
int			expand_meta(char **content, int pos, int r, bool f_quotes);
/*~~~~~~~~~~~~~~~~~~~~~~<utils1_env.c>~~~~~~~~~~~~~~~~~~~~~~*/
void		set_new_content(t_token **token, t_exp *exp, t_ambg *amb);
char		*new_value_quotes(char *str);
/*~~~~~~~~~~~~~~~~~~~~~~<utils_env.c>~~~~~~~~~~~~~~~~~~~~~~*/
void		change_value(bool *quotes, int value);
void		increment(int *v1, int *v2);
void		set_var_exp(int *v1, int *v2, int *v3, int v4);
char		*cdup(int size, char *content);
bool		is_valid_key(char c);
/*~~~~~~~~~~~~~~~~~~~~~~<env_leaks.c>~~~~~~~~~~~~~~~~~~~~~~*/
void		clean_env(t_env *env);
/*~~~~~~~~~~~~~~~~~~~~~~<list_cmd.c>~~~~~~~~~~~~~~~~~~~~~~*/
t_cmd		*create_list_cmd(t_token *tokens);
/*~~~~~~~~~~~~~~~~~~~~~~<utils_cmd.c>~~~~~~~~~~~~~~~~~~~~~~*/
int			counter_pipe(t_token *tokens);
int			analyze_word(t_token *tokens);
bool		mvalloc(char ***commands, int size);
void		amb_next(t_token *token, t_cmd *tmd);
/*~~~~~~~~~~~~~~~~~~~~~~<nodes_cmd.c>~~~~~~~~~~~~~~~~~~~~~~*/
t_cmd		*cmd_node(void);
t_red		*red_node(void);
/*~~~~~~~~~~~~~~~~~~~~~~<content_cmd.c>~~~~~~~~~~~~~~~~~~~~~~*/
bool		hydrate_cmd(t_cmd **cmd, t_token *tokens);
/*~~~~~~~~~~~~~~~~~~~~~~<scrap.c>~~~~~~~~~~~~~~~~~~~~~~*/
bool		all_scrap(t_token *tokens);
/*~~~~~~~~~~~~~~~~~~~~~~<utils_scrap.c>~~~~~~~~~~~~~~~~~~~~~~*/
void		is_vred(t_token *tokens);
void		wr_syntax(t_type type);
/*~~~~~~~~~~~~~~~~~~~~~~<leaks.c>~~~~~~~~~~~~~~~~~~~~~~*/
void		first_free(t_token *tokens, char *input);
void		free_string(char **ptr);
void		p2char(char ***ptr);
/*~~~~~~~~~~~~~~~~~~~~~~<leaks.c>~~~~~~~~~~~~~~~~~~~~~~*/
void		cmdfree(t_cmd *cmd);
/*~~~~~~~~~~~~~~~~~~~~~~<signal.c>~~~~~~~~~~~~~~~~~~~~~~*/
void		set_signals_main(void);
void		set_signals_child(void);
void		set_signals_heredoc(void);
/*~~~~~~~~~~~~~~~~~~~~~~<error.c>~~~~~~~~~~~~~~~~~~~~~~*/
void		eprintf(const char *str);

#endif