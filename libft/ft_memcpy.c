/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarodri2 <rarodri2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 12:26:47 by rarodri2          #+#    #+#             */
/*   Updated: 2025/01/22 12:30:52 by rarodri2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t	i;

	if (!dst && !src)
		return (0);
	i = 0;
	while (i < n)
	{
		((unsigned char *)dst)[i] = ((unsigned char *)src)[i];
		i++;
	}
	return (dst);
}
// dst == null, src == null, n == 3 => Segmentation fault
// dst != null, src == null, n == 3 => Segmentation fault
// dst == null, src != null, n == 3 => Segmentation fault
// dst == null, src == null, n == 0 => (nil)
// dst == null, src != null, n == 0 => (nil)
// dst != null, src == null, n == 0 => dst