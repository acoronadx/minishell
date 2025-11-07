/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_removal.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 13:49:35 by acoronad          #+#    #+#             */
/*   Updated: 2025/11/07 18:55:08 by acoronad         ###   ########.fr       */
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
				{
					i += 2;
					continue ;
				}
				if (s[i + 1])
				{
					i++;
					out[j++] = s[i];
					i++;
					continue ;
				}
				out[j++] = s[i];
				i++;
				continue ;
			}
			if (s[i] == '\'')
			{
				q = SINGLE_QUOTE;
				i++;
				continue ;
			}
			if (s[i] == '"')
			{
				q = DOUBLE_QUOTE;
				i++;
				continue ;
			}
			out[j++] = s[i];
			i++;
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
		if (s[i] == '\\')
		{
			if (s[i + 1] == '\n')
			{
				i += 2;
				continue ;
			}
			if (s[i + 1] && (s[i + 1] == '$' || s[i + 1] == '`' || s[i
					+ 1] == '"' || s[i + 1] == '\\'))
			{
				i++;
				out[j++] = s[i];
				i++;
				continue ;
			}
			out[j++] = s[i];
			i++;
			continue ;
		}
		if (s[i] == '"')
		{
			q = NO_QUOTE;
			i++;
			continue ;
		}
		out[j++] = s[i];
		i++;
	}
	out[j] = '\0';
	return (out);
}

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

char *quote_remove_for_delim(const char *s)
{
    size_t n, i, j;
    char  *out;

    if (!s)
        return NULL;
    n = ft_strlen(s);
    out = (char *)malloc(n + 1);
    if (!out)
        return NULL;

    i = 0; j = 0;
    while (s[i])
    {
        if (s[i] == '\'' || s[i] == '"')
        {
            char q = s[i++];
            while (s[i] && s[i] != q)
                out[j++] = s[i++];
            if (s[i] == q) i++; /* saltar cierre si lo hay */
        }
        else
        {
            out[j++] = s[i++];
        }
    }
    out[j] = '\0';
    return out;
}
