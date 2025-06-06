/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarodri2 <rarodri2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 12:49:40 by rarodri2          #+#    #+#             */
/*   Updated: 2025/01/22 13:10:10 by rarodri2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	unsigned char		*dest;
	const unsigned char	*sr;

	dest = (unsigned char *)dst;
	sr = (const unsigned char *)src;
	if (dst == src)
		return (dst);
	if (dest > sr)
	{
		while (len--)
			dest[len] = sr[len];
	}
	else
	{
		ft_memcpy(dst, src, len);
	}
	return (dst);
}
/*
#include <stdio.h>

int	main(void)
{
	char	str[] = "Hello, world";

	ft_memmove(str + 0, str + 6, 7);
	printf("Result: %s\n", str);
	return (0);
}
*/