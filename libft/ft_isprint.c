/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isprint.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarodri2 <rarodri2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 10:51:45 by rarodri2          #+#    #+#             */
/*   Updated: 2025/01/20 14:32:36 by rarodri2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isprint(int c)
{
	while (c >= 32 && c <= 126)
		return (1);
	return (0);
}
/*
#include <stdio.h>

int	main(void)
{
	char	c1 = 'A';
	char	c2 = '5';
	char	c3 = '@';
	printf("Is %c printable? %d\n", c1, ft_isprint(c1));
	printf("Is %c printable? %d\n", c2, ft_isprint(c2));
	printf("Is %c printable? %d\n", c3, ft_isprint(c3));
	return (0);
}
*/