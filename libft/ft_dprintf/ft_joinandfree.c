/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_joinandfree.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 20:25:07 by acoronad          #+#    #+#             */
/*   Updated: 2025/05/30 12:57:55 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_dprintf.h"

char	*ft_joinandfreestr(char *s1, const char *s2)
{
	char	*str;
	size_t	len1;
	size_t	len2;

	if (!s1 || !s2)
		return (NULL);
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	str = (char *)malloc(len1 + len2 + 1);
	if (!str)
	{
		free(s1);
		return (NULL);
	}
	ft_memcpy(str, s1, len1);
	ft_memcpy(str + len1, s2, len2);
	str[len1 + len2] = '\0';
	free(s1);
	return (str);
}

char	*ft_joinandfree(char *s1, char s2)
{
	char	*str;
	size_t	len1;

	if (!s1)
		return (NULL);
	len1 = ft_strlen(s1);
	str = (char *)malloc(len1 + 2);
	if (!str)
	{
		free(s1);
		return (NULL);
	}
	ft_memcpy(str, s1, len1);
	str[len1] = s2;
	str[len1 + 1] = '\0';
	free(s1);
	return (str);
}