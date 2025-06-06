/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarodri2 <rarodri2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 15:27:02 by rarodri2          #+#    #+#             */
/*   Updated: 2025/03/10 18:04:29 by rarodri2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	**ft_free_mem(char **ptr)
{
	size_t	i;

	i = 0;
	while (ptr[i])
	{
		free(ptr[i]);
		i++;
	}
	free(ptr);
	return (NULL);
}

static size_t	ft_countwords(char const *s, char c)
{
	size_t	count;

	if (!*s)
		return (0);
	count = 0;
	while (*s)
	{
		while (*s == c)
			s++;
		if (*s)
			count++;
		while (*s != c && *s)
			s++;
	}
	return (count);
}

static void	ft_wd(char **str, size_t *w, char c)
{
	size_t	i;

	*str += *w;
	*w = 0;
	i = 0;
	while (**str && **str == c)
		(*str)++;
	while ((*str)[i])
	{
		if ((*str)[i] == c)
			return ;
		(*w)++;
		i++;
	}
}

char	**ft_split(char const *s, char c)
{
	char	**ptr_w;
	char	*str;
	size_t	w;
	size_t	i;

	if (!s)
		return (NULL);
	ptr_w = (char **)malloc(sizeof(char *) * (ft_countwords(s, c) + 1));
	if (!ptr_w)
		return (NULL);
	i = 0;
	str = (char *)s;
	w = 0;
	while (i < ft_countwords(s, c))
	{
		ft_wd(&str, &w, c);
		ptr_w[i] = (char *)malloc(sizeof(char) * (w + 1));
		if (!ptr_w[i])
			return (ft_free_mem(ptr_w));
		ft_strlcpy(ptr_w[i], str, w + 1);
		i++;
	}
	ptr_w[i] = NULL;
	return (ptr_w);
}
