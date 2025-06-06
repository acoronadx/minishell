
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_df.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 12:59:36 by acoronad          #+#    #+#             */
/*   Updated: 2025/05/30 12:59:39 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_dprintf.h"

void	ft_putnbr_bf(char **bf, int n)
{
	char *new;

	if (n == -2147483648)
	{
		new = ft_joinandfreestr(*bf, "-2147483648");
		if (new)
			*bf = new;
		return ;
	}
	if (n < 0)
	{
		new = ft_joinandfree(*bf, '-');
		if (new)
			*bf = new;
		n = -n;
	}
	if (n >= 10)
		ft_putnbr_bf(bf, n / 10);
	new = ft_joinandfree(*bf, '0' + (n % 10));
	if (new)
		*bf = new;
}
