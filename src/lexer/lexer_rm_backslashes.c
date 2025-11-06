/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_rm_backslashes.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 15:38:56 by acoronad          #+#    #+#             */
/*   Updated: 2025/11/06 13:11:16 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* copia el par escapado: avanza i, copia src[i] en out[j], avanza ambos */
static void	copy_escaped_pair(const char *src, size_t *i, char *out, size_t *j)
{
	*i += 1;
	out[*j] = src[*i];
	*j += 1;
	*i += 1;
}

/* bucle de copia dentro de "..." (bash): \ escapa $, `, ", \ y newline */
static void	dq_copy_loop(const char *src, size_t n, char *out, size_t *i, size_t *j)
{
	while (*i < n)
	{
		if (src[*i] == '\\' && (*i + 1) < n
			&& (src[*i + 1] == '$' || src[*i + 1] == '`'
				|| src[*i + 1] == '"' || src[*i + 1] == '\\'))
			copy_escaped_pair(src, i, out, j);
		else if (src[*i] == '\\' && (*i + 1) < n && src[*i + 1] == '\n')
			*i += 2;
		else
		{
			out[*j] = src[*i];
			*j += 1;
			*i += 1;
		}
	}
}

/* bucle de copia fuera de comillas: \x => x ; \newline => se elimina */
static void	unq_copy_loop(const char *src, size_t n, char *out, size_t *i, size_t *j)
{
	while (*i < n)
	{
		if (src[*i] == '\\' && (*i + 1) < n && src[*i + 1] == '\n')
			*i += 2;
		else if (src[*i] == '\\' && (*i + 1) < n)
			copy_escaped_pair(src, i, out, j);
		else
		{
			out[*j] = src[*i];
			*j += 1;
			*i += 1;
		}
	}
}

/* elimina backslashes según el contexto de quote del token */
char	*remove_backslashes_for_token(const char *src, t_quote quote)
{
	char	*out;
	size_t	i;
	size_t	j;
	size_t	n;

	if (!src)
		return (NULL);
	if (quote == SINGLE_QUOTE)
		return (ft_strdup(src));
	n = ft_strlen(src);
	out = (char *)malloc(n + 1);
	if (!out)
		return (NULL);
	i = 0;
	j = 0;
	if (quote == DOUBLE_QUOTE)
		dq_copy_loop(src, n, out, &i, &j);
	else
		unq_copy_loop(src, n, out, &i, &j);
	out[j] = '\0';
	return (out);
}
