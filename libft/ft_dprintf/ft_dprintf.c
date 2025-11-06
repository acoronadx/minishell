/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dprintf.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 17:43:21 by acoronad          #+#    #+#             */
/*   Updated: 2025/05/30 12:56:02 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_dprintf.h"

void	ft_dprintf(int fd, const char *format, ...)
{
	va_list args;
	char *bf;
	int i;

	if (!format)
		return ;
	bf = ft_strdup("");
	if (!bf)
		return ;
	va_start(args, format);
	i = 0;
	while (format[i])
	{
		if (format[i] == '%' && format[i + 1])
		{
			ft_dprintftype(&bf, format[i + 1], args);
			i++;
		}
		else
			bf = ft_joinandfree(bf, format[i]);
		if (!bf)
		{
			va_end(args);
			return ;
		}
		i++;
	}
	va_end(args);
	write(fd, bf, ft_strlen(bf));
	free(bf);
}