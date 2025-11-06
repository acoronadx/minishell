/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_rm_backslashes.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 15:38:56 by acoronad          #+#    #+#             */
/*   Updated: 2025/11/05 15:40:02 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	copy_escaped_pair(const char *src, size_t *i, char *out, size_t *j)
{
	(*i)++;
	out[*j] = src[*i];
	(*j)++;
	(*i)++;
}

char	*remove_backslashes_for_token(const char *src, t_quote quote)
{
	char	*out;
	size_t	i;
	size_t	j;
	size_t	n;

	if (!src)
		return (NULL);
	n = ft_strlen(src);
	out = (char *)malloc(n + 1);
	if (!out)
		return (NULL);
	i = 0;
	j = 0;
	if (quote == SINGLE_QUOTE)
	{
		while (i < n)
		{
			out[j] = src[i];
			j++;
			i++;
		}
		out[j] = '\0';
		return (out);
	}
	if (quote == DOUBLE_QUOTE)
	{
		while (i < n)
		{
			if (src[i] == '\\' && (i + 1) < n && (src[i + 1] == '$' || src[i
					+ 1] == '`' || src[i + 1] == '"' || src[i + 1] == '\\'))
				copy_escaped_pair(src, &i, out, &j);
			else if (src[i] == '\\' && (i + 1) < n && src[i + 1] == '\n')
				i += 2;
			else
			{
				out[j] = src[i];
				j++;
				i++;
			}
		}
		out[j] = '\0';
		return (out);
	}
	while (i < n)
	{
		if (src[i] == '\\' && (i + 1) < n && src[i + 1] == '\n')
			i += 2;
		else if (src[i] == '\\' && (i + 1) < n)
			copy_escaped_pair(src, &i, out, &j);
		else
		{
			out[j] = src[i];
			j++;
			i++;
		}
	}
	out[j] = '\0';
	return (out);
}
