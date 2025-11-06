
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putptr_bf.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 22:29:17 by acoronad          #+#    #+#             */
/*   Updated: 2025/05/30 12:58:26 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_dprintf.h"
#include <stdint.h>

static void	ft_putnbr_x_bf(char **bf, uintptr_t ptr)
{
	const char	*base = "0123456789abcdef";
	char		*new;

	if (ptr >= 16)
		ft_putnbr_x_bf(bf, ptr / 16);
	new = ft_joinandfree(*bf, base[ptr % 16]);
	if (new)
		*bf = new;
}

void	ft_putptr_bf(char **bf, void *ptr)
{
	if (!ptr)
	{
		ft_dputstr_bf(bf, "(nil)");
		return ;
	}
	ft_dputstr_bf(bf, "0x");
	ft_putnbr_x_bf(bf, (uintptr_t)ptr);
}