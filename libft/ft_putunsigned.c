/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putunsigned.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarodri2 <rarodri2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 13:02:24 by rarodri2          #+#    #+#             */
/*   Updated: 2025/02/27 15:59:19 by rarodri2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putunsigned(unsigned int n)
{
	int		len;
	char	number;

	len = 0;
	if (n >= 10)
		len += ft_putnbr(n / 10);
	number = (n % 10) + '0';
	write(1, &number, 1);
	len++;
	return (len);
}
