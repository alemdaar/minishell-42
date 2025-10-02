/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelhasso <oelhasso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 20:19:07 by oelhasso          #+#    #+#             */
/*   Updated: 2025/10/02 21:35:49 by oelhasso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
#include "../header.h"

static int	ft_atoi(const char *str)
{
	int					i;
	int					sign;
	unsigned long long	result;

	i = 0;
	sign = 1;
	result = 0;
	while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign *= -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		if (sign == 1 && result > 9223372036854775807)
			return (-1);
		if (sign == -1 && result > 9223372036854775807)
			return (0);
		result = (result * 10) + (str[i] - 48);
		i++;
	}
	return ((int)(result * sign));
}

static int	builtin_exit2(t_cmd *tmp, int *i)
{
	printf("exit\n");
	if (!tmp->commands[1])
		exit(exit_status(-1));
	*i = 0;
	if (!tmp->commands[1][0])
		return (print_err("exit", tmp->commands[1], EX_NA), exit(255), 1);
	while (tmp->commands[1][*i] == ' ')
		*i += 1;
	if (tmp->commands[1][*i] == '+' || tmp->commands[1][*i] == '-')
	{
		*i += 1;
	}
	if (!ft_isdigit(tmp->commands[1][*i]))
	{
		return (print_err("exit", tmp->commands[1], EX_NA), exit(255), 1);
	}
	return (0);
}

int	builtin_exit(t_cmd *tmp)
{
	int	i;

	builtin_exit2(tmp, &i);
	while (tmp->commands[1][i])
	{
		if (tmp->commands[1][i] == ' ')
		{
			while (tmp->commands[1][i] == ' ')
				i++;
			break ;
		}
		if (!ft_isdigit(tmp->commands[1][i]) && tmp->commands[1][i])
			return (print_err("exit", tmp->commands[1], EX_NA), exit(255), 1);
		i++;
	}
	while (tmp->commands[1][i] == ' ')
		break ;
	if (tmp->commands[1][i] != 0)
		return (print_err("exit", tmp->commands[1], EX_NA), exit(255), 1);
	if (tmp->commands[2])
		return (print_err("exit", tmp->commands[1], EX_TA), exit_status(1), 1);
	exit((unsigned char)ft_atoi(tmp->commands[1]));
}
