/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarodri2 <rarodri2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 13:02:24 by rarodri2          #+#    #+#             */
/*   Updated: 2025/02/27 15:59:00 by rarodri2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putnbr(int nb)
{
	int		len;
	char	number;

	len = 0;
	if (nb == -2147483648)
	{
		write(1, "-2147483648", 11);
		return (11);
	}
	if (nb < 0)
	{
		write (1, "-", 1);
		nb = -nb;
		len++;
	}
	if (nb >= 10)
		len += ft_putnbr(nb / 10);
	number = (nb % 10) + '0';
	write(1, &number, 1);
	len++;
	return (len);
}
