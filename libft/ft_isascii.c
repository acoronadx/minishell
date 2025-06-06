/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isascii.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarodri2 <rarodri2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 16:28:53 by rarodri2          #+#    #+#             */
/*   Updated: 2025/01/22 11:05:02 by rarodri2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isascii(int c)
{
	return (c >= 0 && c <= 127);
}
/*
#include <stdio.h>
int	main(void)
{
	char	c1 = 'A';
	char	c2 = '5';
	char	c3 = '@';
	printf("Is %c ascii? %d\n", c1, ft_isascii(c1));
	printf("Is %c ascii? %d\n", c2, ft_isascii(c2));
	printf("Is %c ascii? %d\n", c3, ft_isascii(c3));
	return (0);
}
*/