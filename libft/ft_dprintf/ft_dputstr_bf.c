
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dputstr_bf.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 20:01:57 by acoronad          #+#    #+#             */
/*   Updated: 2025/05/30 12:57:08 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_dprintf.h"

void	ft_dputstr_bf(char **bf, const char *s)
{
	char *new;

	if (!s)
		new = ft_joinandfreestr(*bf, "(null)");
	else
		new = ft_joinandfreestr(*bf, s);
	if (new)
		*bf = new;
}
