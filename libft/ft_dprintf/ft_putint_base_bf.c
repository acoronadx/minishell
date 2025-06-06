
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putint_base_bf.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 15:00:20 by acoronad          #+#    #+#             */
/*   Updated: 2025/05/30 12:58:13 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_dprintf.h"

static int	ft_check_base(const char *base)
{
	int		i;
	int		j;

	if (!base || base[0] == '\0' || base[1] == '\0')
		return (0);
	i = 0;
	while (base[i] != '\0')
	{
		if (base[i] == '+' || base[i] == '-')
			return (0);
		j = i + 1;
		while (base[j] != '\0')
		{
			if (base[i] == base[j])
				return (0);
			j++;
		}
		i++;
	}
	return (i);
}

void	ft_pint_bs_bf(char **bf, unsigned int nbr, const char *base)
{
	int	base_ln;
	char *new;

	base_ln = ft_check_base(base);
	if (base_ln <= 1)
		return ;
	if (nbr >= (unsigned int)base_ln)
		ft_pint_bs_bf(bf, nbr / base_ln, base);
	new = ft_joinandfree(*bf, base[nbr % base_ln]);
	if (new)
		*bf = new;
}