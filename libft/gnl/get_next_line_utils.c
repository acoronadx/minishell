/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 14:51:37 by acoronad          #+#    #+#             */
/*   Updated: 2025/02/07 14:17:26 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

size_t	ft_position(char *s, char c)
{
	size_t	i;

	i = 0;
	while (s[i] != '\0')
	{
		if (s[i] == c)
			return (i);
		i++;
	}
	return (i);
}

size_t	ft_strlen(const char *s)
{
	size_t	i;

	if (!s)
		return (0);
	i = 0;
	while (s[i] != '\0')
		i++;
	return (i);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*dest;
	char	*str;
	char	*s1aux;
	char	*s2aux;

	s1aux = (char *)s1;
	s2aux = (char *)s2;
	if ((s1 == NULL) && (s2 == NULL))
		return (NULL);
	if (s2 == NULL)
		return (ft_strldup(s1, ft_strlen(s1)));
	if (s1 == NULL)
		return (ft_strldup(s2, ft_strlen(s2)));
	dest = (char *)malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	str = dest;
	if (!dest)
		return (NULL);
	while (*s1aux)
		*dest++ = *s1aux++;
	while (*s2aux)
		*dest++ = *s2aux++;
	*dest = '\0';
	return (str);
}

char	*ft_strldup(const char *s, size_t size)
{
	char	*str;
	size_t	i;

	i = 0;
	if (s == NULL)
		return (NULL);
	str = (char *)malloc(sizeof(char) * (size + 1));
	if (str == NULL)
		return (NULL);
	while (i < size)
	{
		str[i] = s[i];
		i++;
	}
	str[i] = '\0';
	return (str);
}
