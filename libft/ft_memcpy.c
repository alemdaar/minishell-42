/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelhasso <oelhasso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 19:26:03 by oelhasso          #+#    #+#             */
/*   Updated: 2025/09/14 19:26:04 by oelhasso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	unsigned char	*ma;
	unsigned char	*mi;

	if (dst == src)
		return (dst);
	ma = (unsigned char *)dst;
	mi = (unsigned char *)src;
	while (n--)
		*ma++ = *mi++;
	return (dst);
}
