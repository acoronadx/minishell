
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin_free.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 16:12:06 by acoronad          #+#    #+#             */
/*   Updated: 2025/06/03 16:12:16 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin_free_s1(char *s1, char *s2)
{
	char	*joined;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		return (ft_strdup(s2));
	if (!s2)
	{
		free(s1);
		return (NULL);
	}
	joined = malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (!joined)
		return (NULL);
	ft_strcpy(joined, s1);
	ft_strcat(joined, s2);
	free(s1);
	return (joined);
}

char	*ft_strjoin_free_s2(char *s1, char *s2)
{
	char	*joined;

	if (!s1 && !s2)
		return (NULL);
	if (!s2)
		return (ft_strdup(s1));
	if (!s1)
	{
		free(s2);
		return (NULL);
	}
	joined = malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (!joined)
		return (NULL);
	ft_strcpy(joined, s1);
	ft_strcat(joined, s2);
	free(s2);
	return (joined);
}
char	*ft_strjoin_free_both(char *s1, char *s2)
{
	char	*joined;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		return (ft_strdup(s2));
	if (!s2)
	{
		free(s1);
		return (NULL);
	}
	joined = malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (!joined)
		return (NULL);
	ft_strcpy(joined, s1);
	ft_strcat(joined, s2);
	free(s1);
	free(s2);
	return (joined);
}

char *ft_strjoin_three(const char *s1, const char *s2, const char *s3)
{
    char *new_str;
    size_t len1 = s1 ? ft_strlen(s1) : 0;
    size_t len2 = s2 ? ft_strlen(s2) : 0;
    size_t len3 = s3 ? ft_strlen(s3) : 0;

    new_str = (char *)malloc(sizeof(char) * (len1 + len2 + len3 + 1));
    if (!new_str)
        return (NULL);
    
    new_str[0] = '\0';

    if (s1) ft_strcat(new_str, s1);
    if (s2) ft_strcat(new_str, s2);
    if (s3) ft_strcat(new_str, s3);

    return (new_str);
}
