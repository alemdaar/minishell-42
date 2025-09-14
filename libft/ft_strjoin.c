/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelhasso <oelhasso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 19:26:30 by oelhasso          #+#    #+#             */
/*   Updated: 2025/09/14 19:26:31 by oelhasso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	mlen1;
	size_t	mlen2;
	char	*ma;

	if (!s1 || !s2)
	{
		if (!s1 && s2)
			return ((char *)s2);
		if (!s2 && s1)
			return ((char *)s1);
		return (NULL);
	}
	mlen1 = ft_strlen(s1);
	mlen2 = ft_strlen(s2);
	ma = (char *)malloc(sizeof(char) * (mlen1 + mlen2 + 1));
	if (!ma)
		return (NULL);
	ft_memcpy(ma, s1, mlen1);
	ft_memcpy(ma + mlen1, s2, mlen2);
	ma[mlen1 + mlen2] = '\0';
	return (ma);
}
