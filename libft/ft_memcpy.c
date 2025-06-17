/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbarhoun <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 21:30:24 by mbarhoun          #+#    #+#             */
/*   Updated: 2024/10/31 11:01:46 by mbarhoun         ###   ########.fr       */
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
