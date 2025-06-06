/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalnum.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarodri2 <rarodri2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 16:21:38 by rarodri2          #+#    #+#             */
/*   Updated: 2025/01/20 16:17:47 by rarodri2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isalnum(int c)
{
	if ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z')
		|| (c >= 'A' && c <= 'Z'))
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
	printf("Is %c alphanumeric? %d\n", c1, ft_isalnum(c1));
	printf("Is %c alphanumeric? %d\n", c2, ft_isalnum(c2));
	printf("Is %c alphanumeric? %d\n", c3, ft_isalnum(c3));
	return (0);
}
*/