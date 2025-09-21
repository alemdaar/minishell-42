/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelhasso <oelhasso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 12:39:47 by oelhasso          #+#    #+#             */
/*   Updated: 2025/09/21 22:43:23 by oelhasso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "./header.h"

int file_pipe (t_cmd *tmp, int flag)
{
    if (flag == 0)
    {
    	tmp->open2 = tmp->pipefd[WRITE];
    	close(tmp->pipefd[READ]);
    }
    else if (tmp->next == NULL)
    	tmp->open1 = tmp->prev->pipefd[READ];
    else
    {
    	tmp->open1 = tmp->prev->pipefd[READ];
    	tmp->open2 = tmp->pipefd[WRITE];
    	close(tmp->pipefd[READ]);
    }
    return (0);
}