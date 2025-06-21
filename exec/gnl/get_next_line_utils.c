/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelhasso <elhassounioussama2@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 20:58:04 by oelhasso          #+#    #+#             */
/*   Updated: 2024/12/04 21:21:17 by oelhasso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

size_t	ft_strlen(char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

int	ft_strchr(char *str, char c)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == c)
			return (i);
		i++;
	}
	return (-1);
}

void	ft_strlcpy(char *dest, char *src, int add_bytes)
{
	size_t	i;
	size_t	dest_len;

	i = 0;
	dest_len = ft_strlen(dest);
	if (dest_len > 0)
	{
		while (src[i] && i < dest_len)
		{
			dest[i] = src[i];
			i++;
		}
	}
	while (src[i])
		dest[dest_len++] = src[i++];
	while (add_bytes > 0)
	{
		dest[dest_len] = 0;
		dest_len++;
		add_bytes--;
	}
	dest[dest_len] = 0;
}

char	*rtn_line(char *buffer, int p)
{
	char	*str;
	size_t	i;

	i = 0;
	if (p == -1)
		p = (ft_strlen(buffer)) - 1;
	str = malloc (p + 2);
	if (!str)
		return (free_str(&buffer), NULL);
	while ((int)i <= p)
	{
		str[i] = buffer[i];
		i++;
	}
	str[i] = 0;
	return (free_str(&buffer), str);
}

char	*ft_strdup_add(char *src, int add_bytes)
{
	char	*ptr;
	int		src_len;
	int		i;

	i = 0;
	src_len = ft_strlen(src);
	ptr = (char *) malloc(src_len + add_bytes + 1);
	if (ptr == NULL)
		return (NULL);
	while (i <= src_len + add_bytes)
	{
		ptr[i] = 0;
		i++;
	}
	i = 0;
	while (src[i])
	{
		ptr[i] = src[i];
		i++;
	}
	return (ptr);
}
