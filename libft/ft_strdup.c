/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelhasso <oelhasso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 19:25:54 by oelhasso          #+#    #+#             */
/*   Updated: 2025/09/29 17:17:38 by oelhasso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>
char	*ft_strdup(const char *s1)
{
	char	*ma;
	size_t	i;

	i = 0;
	ma = (char *)malloc(sizeof(char) * (ft_strlen(s1) + 1));
	if (!ma)
		return (NULL);
	printf ("strdup : %p\n", ma);
	while (s1[i])
	{
		ma[i] = s1[i];
		i++;
	}
	ma[i] = '\0';
	return (ma);
}
