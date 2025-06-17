/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbarhoun <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 12:14:40 by mbarhoun          #+#    #+#             */
/*   Updated: 2024/11/04 13:07:40 by mbarhoun         ###   ########.fr       */
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
