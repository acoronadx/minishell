/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ispositivfdstr.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 18:14:50 by acoronad          #+#    #+#             */
/*   Updated: 2025/07/03 18:14:52 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_isposfdstr(const char *str)
{
	int	i;

	i = 0;
	if (!str || !str[0])
		return (0);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}
