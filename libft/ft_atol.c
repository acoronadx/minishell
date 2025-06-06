/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarodri2 <rarodri2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 13:27:01 by rarodri2          #+#    #+#             */
/*   Updated: 2025/03/20 13:07:58 by rarodri2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	process_sign_and_spaces(const char *str, size_t *i)
{
	int	sign;

	sign = 1;
	while (str[*i] == ' ' || str[*i] == '\t' || str[*i] == '\n'
		|| str[*i] == '\r' || str[*i] == '\v' || str[*i] == '\f')
		(*i)++;
	if (str[*i] == '-')
	{
		sign = -1;
		(*i)++;
	}
	else if (str[*i] == '+')
		(*i)++;
	return (sign);
}

long	check_overflow(long result, int digit, int sign)
{
	if (result > LONG_MAX / 10 || (result == LONG_MAX / 10 && digit > LONG_MAX
			% 10))
	{
		if (sign == 1)
			return (LONG_MAX);
		else
			return (LONG_MIN);
	}
	return (0);
}

long	ft_atol(const char *str)
{
	long	result;
	size_t	i;
	int		sign;
	long	overflow_check;

	result = 0;
	i = 0;
	sign = process_sign_and_spaces(str, &i);
	while (str[i] >= '0' && str[i] <= '9')
	{
		overflow_check = check_overflow(result, str[i] - '0', sign);
		if (overflow_check != 0)
			return (overflow_check);
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result * sign);
}
