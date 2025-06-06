
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putunsignedint_bf.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 21:20:55 by acoronad          #+#    #+#             */
/*   Updated: 2025/05/30 12:58:51 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_dprintf.h"

void	ft_putunsignedint_bf(char **bf, unsigned int n)
{
	char *new;

	if (n >= 10)
		ft_putunsignedint_bf(bf, n / 10);
	new = ft_joinandfree(*bf, '0' + (n % 10));
	if (new)
		*bf = new;
}