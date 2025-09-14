/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_tokens.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelhasso <oelhasso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 18:06:12 by oelhasso          #+#    #+#             */
/*   Updated: 2025/09/14 18:33:13 by oelhasso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	cut_value(char *tokens, char c, bool f_quotes)
{
	int		r;
	bool	flag;

	r = -1;
	flag = 1;
	if (f_quotes == 1)
	{
		cut_value_quotes(&r, tokens, c, flag);
		return (r);
	}
	while (tokens[++r])
	{
		if (tokens[r] == '"' || tokens[r] == '\'')
		{
			c = tokens[r++];
			while (tokens[r] && tokens[r] != c)
				r++;
		}
		if (is_symbols(tokens[r]) || is_space(tokens[r]))
			break ;
	}
	return (r);
}

static t_type	type(char *content)
{
	if ((content[0] == '"' && has_env(content)) || content[0] == '$')
		return (ENV);
	if (content[0] == '|')
		return (PIPE);
	if (content[0] == '>' && content[1] != '>')
		return (REDIR_OUT);
	if (content[0] == '>' && content[1] == '>')
		return (APPEND);
	if (content[0] == '<' && content[1] != '<')
		return (REDIR_IN);
	if (content[0] == '<' && content[1] == '<')
		return (HERDOOC);
	else
		return (WORD);
}

static void	build_list(t_token **list, char *content)
{
	t_token	*new;

	new = create_node(content);
	if (!*list)
		*list = new;
	else
		node_back(list, new);
	new->type_token = type(new->content);
}

static int	symbols_token(char **new_token, char *tokens, int i, t_token **head)
{
	int	r;

	r = 0;
	if (!is_symbols(tokens[i]))
		return (0);
	if (tokens[i] == '|')
		r++;
	else if ((tokens[i] == '>' && tokens[i + 1] == '>') 
		|| (tokens[i] == '<' && tokens[i + 1] == '<'))
		r += 2;
	else if ((tokens[i] == '>' && tokens[i + 1] != '>') 
		|| (tokens[i] == '<' && tokens[i + 1] != '<'))
		r++;
	*new_token = malloc(sizeof(char) * (r + 1));
	if (!*new_token)
		return (-1);
	ft_strncpy(*new_token, tokens + i, r);
	(*new_token)[r] = '\0';
	build_list(head, *new_token);
	return (r);
}

t_token	*segment_input(char *tokens)
{
	t_spl	spl;
	t_token	*head;

	set_var_zero(&spl.i, &spl.r, &spl.f_quotes, &head);
	while (tokens[spl.i] && spl.i < (int)ft_strlen(tokens))
	{
		spl.i += skip_space(tokens + spl.i);
		spl.i += symbols_token(&spl.new_token, tokens, spl.i, &head);
		spl.i += skip_space(tokens + spl.i);
		if (is_symbols(tokens[spl.i]))
			continue ;
		if (spl.i >= (int)ft_strlen(tokens) || spl.i < 0)
			break ;
		if (tokens[spl.i] == '"' || tokens[spl.i] == '\'')
			spl.f_quotes = 1;
		spl.r = cut_value(tokens + spl.i, tokens[spl.i], spl.f_quotes);
		spl.new_token = malloc(sizeof(char) * (spl.r + 1));
		if (!spl.new_token)
			return (eprintf(ERR_MEM), NULL);
		ft_strncpy(spl.new_token, tokens + spl.i, spl.r);
		spl.new_token[spl.r] = '\0';
		build_list(&head, spl.new_token);
		spl.i += add_value(spl.r, &spl.f_quotes);
	}
	return (head);
}
