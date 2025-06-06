
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dputchar_bf.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 19:56:36 by acoronad          #+#    #+#             */
/*   Updated: 2025/05/30 12:56:40 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_dprintf.h"

void	ft_dputchar_bf(char **bf, char c)
{
	char *new;

	new = ft_joinandfree(*bf, c);
	if (new)
		*bf = new;
}