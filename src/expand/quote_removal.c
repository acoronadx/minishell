/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_removal.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 16:58:00 by acoronad          #+#    #+#             */
/*   Updated: 2025/11/08 17:32:46 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*quote_remove_like_bash(const char *s)
{
	size_t	n;
	char	*out;
	int		i;
	int		j;
	t_quote	q;

	if (!s)
		return (NULL);
	n = ft_strlen(s);
	out = (char *)malloc(n + 1);
	if (!out)
		return (NULL);
	i = 0;
	j = 0;
	q = NO_QUOTE;
	while (s[i])
	{
		if (q == NO_QUOTE)
		{
			if (s[i] == '\\')
			{
				if (s[i + 1] == '\n')
					i += 2;
				else if (s[i + 1])
				{
					i++;
					out[j++] = s[i];
					i++;
				}
				else
				{
					out[j++] = s[i];
					i++;
				}
				continue ;
			}
			if (s[i] == '\'')
			{
				i++;
				q = SINGLE_QUOTE;
				continue ;
			}
			if (s[i] == '"')
			{
				i++;
				q = DOUBLE_QUOTE;
				continue ;
			}
			out[j++] = s[i++];
			continue ;
		}
		if (q == SINGLE_QUOTE)
		{
			if (s[i] == '\'')
				q = NO_QUOTE;
			else
				out[j++] = s[i];
			i++;
			continue ;
		}
		/* q == DOUBLE_QUOTE */
		if (s[i] == '\\')
		{
			if (s[i + 1] == '\n')
				i += 2;
			else if (s[i + 1] && (s[i + 1] == '$' || s[i + 1] == '`'
					|| s[i + 1] == '"' || s[i + 1] == '\\'))
			{
				i++;
				out[j++] = s[i];
				i++;
			}
			else
				out[j++] = s[i++];
			continue ;
		}
		if (s[i] == '"')
		{
			i++;
			q = NO_QUOTE;
			continue ;
		}
		out[j++] = s[i++];
	}
	out[j] = '\0';
	return (out);
}

/* Para tokens completos, si la usas */
void	remove_quotes(t_token *tokens)
{
	t_token	*t;
	char	*clean;

	t = tokens;
	while (t)
	{
		clean = quote_remove_like_bash(t->value);
		if (!clean)
			return ;
		free(t->value);
		t->value = clean;
		t->quoted = NO_QUOTE;
		t = t->next;
	}
}

/* Quita comillas SOLO para delimitador de heredoc (sin semántica de \) */
char	*quote_remove_for_delim(const char *s)
{
	size_t	i;
	size_t	j;
	size_t	n;
	char	*out;

	if (!s)
		return (NULL);
	n = ft_strlen(s);
	out = (char *)malloc(n + 1);
	if (!out)
		return (NULL);
	i = 0;
	j = 0;
	while (s[i])
	{
		if (s[i] == '\'' || s[i] == '"')
		{
			char	q = s[i++];
			while (s[i] && s[i] != q)
				out[j++] = s[i++];
			if (s[i] == q)
				i++;
		}
		else
			out[j++] = s[i++];
	}
	out[j] = '\0';
	return (out);
}
