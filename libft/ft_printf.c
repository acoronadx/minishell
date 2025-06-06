/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarodri2 <rarodri2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 12:32:20 by rarodri2          #+#    #+#             */
/*   Updated: 2025/02/07 12:32:20 by rarodri2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_conversion(char c, va_list list_of_args)
{
	if (c == 'c')
		return (ft_putchar(va_arg(list_of_args, int)));
	else if (c == 's')
		return (ft_putstr(va_arg(list_of_args, char *)));
	else if (c == 'p')
		return (ft_putptr(va_arg(list_of_args, void *)));
	else if (c == 'd' || c == 'i')
		return (ft_putnbr(va_arg(list_of_args, int)));
	else if (c == 'u')
		return (ft_putunsigned(va_arg(list_of_args, unsigned int)));
	else if (c == 'x')
		return (ft_puthex(va_arg(list_of_args, unsigned int), 0));
	else if (c == 'X')
		return (ft_puthex(va_arg(list_of_args, unsigned int), 1));
	else if (c == '%')
		return (ft_putchar('%'));
	return (0);
}

int	ft_printf(char const *format, ...)
{
	va_list	list_of_args;
	int		len;
	int		i;

	va_start(list_of_args, format);
	len = 0;
	i = 0;
	while (format[i])
	{
		if (format[i] == '%')
		{
			len += ft_conversion(format[i + 1], list_of_args);
			i++;
		}
		else
		{
			len += ft_putchar(format[i]);
		}
		if (format[i])
			i++;
	}
	va_end(list_of_args);
	return (len);
}
