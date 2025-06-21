/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelhasso <elhassounioussama2@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 22:48:04 by oelhasso          #+#    #+#             */
/*   Updated: 2025/02/21 17:48:03 by oelhasso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

void	free_str(char **str)
{
	if (str && *str)
	{
		free (*str);
		*str = NULL;
	}
}

int	new_line_check(char *buffer)
{
	int	i;

	i = 0;
	while (buffer[i])
	{
		if (buffer[i] == '\n')
			return (1);
		i++;
	}
	return (0);
}

char	*return_new_line(char *buffer, char **left_address)
{
	char	*str;
	int		p;

	p = ft_strchr(buffer, '\n');
	if (p == -1)
	{
		buffer = rtn_line(buffer, (int)(ft_strlen(buffer)) - 1);
		return (free_str(left_address), buffer);
	}
	*left_address = ft_strdup_add(buffer + p + 1, 0);
	str = rtn_line(buffer, p);
	if (!str)
		free_str(left_address);
	return (str);
}

char	*read_it(char *buffer, int fd, char **left)
{
	ssize_t	r;
	char	*tmp;
	int		len;

	while (1)
	{
		len = ft_strlen(buffer);
		r = read(fd, buffer + len, BUFFER_SIZE);
		if (r == 0)
			break ;
		buffer[len + r] = '\0';
		if (new_line_check(buffer))
			break ;
		tmp = ft_strdup_add(buffer, BUFFER_SIZE);
		free_str(&buffer);
		if (!tmp)
			return (NULL);
		buffer = tmp;
		tmp = NULL;
	}
	if (r == 0 && (!buffer || !*buffer))
		return (free_str(&buffer), NULL);
	return (return_new_line(buffer, left));
}

char	*get_next_line(int fd)
{
	static char	*left;
	char		*buffer;
	char		*rtn;

	if (!left)
	{
		left = ft_strdup_add("", 0);
		if (!left)
			return (NULL);
	}
	if (fd < 0 || BUFFER_SIZE < 0 || read (fd, 0, 0) == -1)
		return (free_str(&left), NULL);
	buffer = ft_strdup_add(left, BUFFER_SIZE);
	free_str(&left);
	if (!buffer)
		return (free_str(&left), NULL);
	rtn = read_it(buffer, fd, &left);
	return (rtn);
}
