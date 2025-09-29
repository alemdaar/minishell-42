/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelhasso <oelhasso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 19:26:43 by oelhasso          #+#    #+#             */
/*   Updated: 2025/09/29 17:09:54 by oelhasso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*ma;
	size_t	mlen;
	size_t	b;

	if (!s)
		return (NULL);
	mlen = ft_strlen(s);
	b = 0;
	if (start >= (unsigned int)mlen)
		return (ft_strdup(""));
	if (len > mlen - start)
		len = mlen - start;
	ma = (char *)malloc(sizeof(char) * (len + 1));
	if (!ma)
		return (NULL);
	printf ("substr : %p\n", ma);
	while (b < len)
	{
		ma[b] = s[start + b];
		b++;
	}
	ma[b] = '\0';
	return (ma);
}
