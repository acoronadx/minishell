/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_puthex.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarodri2 <rarodri2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 13:03:33 by rarodri2          #+#    #+#             */
/*   Updated: 2025/02/27 15:57:46 by rarodri2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_puthex(unsigned long n, int uppercase)
{
	int		len;
	char	*base;

	len = 0;
	if (n == 0)
	{
		ft_putchar('0');
		return (1);
	}
	if (uppercase)
		base = "0123456789ABCDEF";
	else
		base = "0123456789abcdef";
	if (n >= 16)
		len += ft_puthex(n / 16, uppercase);
	ft_putchar(base[n % 16]);
	len++;
	return (len);
}
