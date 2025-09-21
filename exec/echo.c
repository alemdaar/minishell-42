/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelhasso <oelhasso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 19:57:19 by oelhasso          #+#    #+#             */
/*   Updated: 2025/09/21 20:45:31 by oelhasso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "./header.h"

static int echo_nl(t_cmd *tmp, int ind)
{
	while (tmp->commands[ind])
	{
		int i = 0;
		if (tmp->commands[ind][i++] != '-')
			return (ind);
		if (!tmp->commands[ind][i])
			return (ind);
		while (tmp->commands[ind][i])
		{
			if (tmp->commands[ind][i] != 'n')
				return (ind);
			i++;
		}
		ind++;
	}
	return (ind);
}

static void builtin_echo2(t_cmd *tmp, int *c, int *j, int newline)
{
    while (tmp->commands[*j])
	{
		if (*c == 1)
			printf (" ");
		printf ("%s", tmp->commands[*j++]);
		*c = 1;
	}
    if (newline)
        printf("\n");
    return ;
}

int builtin_echo(t_cmd *tmp)
{
    int i;
    int j;
    int c;
    int newline;

    i = 1;
    j = 1;
    c = 0;
    newline = 1;
	if (tmp->commands[i])
    {
		if (tmp->commands[i][0])
		{
			j = echo_nl(tmp, i);
			if (j > i)
        		newline = 0;
		}
    }
    builtin_echo2(tmp, &c, &j, newline);
    return (SUCCESSFUL);
}

