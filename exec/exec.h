/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelhasso <oelhasso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 17:28:22 by oelhasso          #+#    #+#             */
/*   Updated: 2025/06/18 20:46:49 by oelhasso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H


#include "../minishell.h"

typedef struct s_info {
    int a_pipe;
}	t_info;

int exec (t_cmd *cmd, t_env *env);

#endif