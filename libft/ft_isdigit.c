/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isdigit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarodri2 <rarodri2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 16:09:41 by rarodri2          #+#    #+#             */
/*   Updated: 2025/01/20 14:31:15 by rarodri2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}
/*
#include <stdio.h>

int	main(void)
{
	char	c1;
	char	c2;
	char	c3;

	c1 = 'A';
	c2 = '5';
	c3 = '@';
	printf("Is %c a digit? %d\n", c1, ft_isdigit(c1));
	printf("Is %c a digit? %d\n", c2, ft_isdigit(c2));
	printf("Is %c a digit? %d\n", c3, ft_isdigit(c3));
	return (0);
}
*/