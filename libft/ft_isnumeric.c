/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isnumeric.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 05:01:58 by acoronad          #+#    #+#             */
/*   Updated: 2025/06/26 05:02:00 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "libft.h"

int ft_isnumeric(const char *str)
{
	if (*str == '-' || *str == '+')
		str++;
	if (!*str)
		return (0); // No es numérico si está vacío
	while (*str)
	{
		if (!ft_isdigit(*str))
			return (0); // No es numérico si hay un carácter no dígito
		str++;
	}
	return (1); // Es numérico
}
