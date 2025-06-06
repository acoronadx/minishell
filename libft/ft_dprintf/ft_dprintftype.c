
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dprintftype.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 13:05:12 by acoronad          #+#    #+#             */
/*   Updated: 2025/05/30 12:56:18 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_dprintf.h"

void	ft_dprintftype(char **bf, char type, va_list put)
{
	if (type == 'p')
		ft_putptr_bf(bf, va_arg(put, void *));
	else if (type == 's')
		ft_dputstr_bf(bf, va_arg(put, char *));
	else if (type == 'c')
		ft_dputchar_bf(bf, va_arg(put, int));
	else if (type == 'x')
		ft_pint_bs_bf(bf, va_arg(put, unsigned int), "0123456789abcdef");
	else if (type == 'X')
		ft_pint_bs_bf(bf, va_arg(put, unsigned int), "0123456789ABCDEF");
	else if (type == 'i' || type == 'd')
		ft_putnbr_bf(bf, va_arg(put, int));
	else if (type == 'u')
		ft_putunsignedint_bf(bf, va_arg(put, unsigned int));
	else if (type == '%')
		ft_dputchar_bf(bf, '%');
}