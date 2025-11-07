/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 22:41:05 by acoronad          #+#    #+#             */
/*   Updated: 2025/11/07 23:22:31 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* helpers: ≤25 líneas cada función, sin for/ternario */

static size_t	var_len(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i] && (ft_isalnum((int)s[i]) || s[i] == '_'))
		i++;
	return (i);
}

static int	append_sub(char **dst, const char *src, size_t n)
{
	char	*cut;
	char	*join;

	cut = NULL;
	join = NULL;
	cut = ft_substr(src, 0, n);
	if (!cut)
		return (-1);
	join = ft_strjoin(*dst, cut);
	free(cut);
	if (!join)
		return (-1);
	free(*dst);
	*dst = join;
	return (0);
}

static int	append_cstr(char **dst, const char *s)
{
	size_t	n;

	if (!s)
		return (0);
	n = ft_strlen(s);
	return (append_sub(dst, s, n));
}

/* Solo expansión de parámetros en heredoc no citado. */
char	*expand_heredoc_line(t_shell *sh, const char *line)
{
	size_t	i;
	size_t	n;
	char	*res;
	char	*name;
	char	*val;

	i = 0;
	res = NULL;
	name = NULL;
	val = NULL;
	res = ft_strdup("");
	if (!res)
		return (NULL);
	while (line[i])
	{
		if (line[i] == '\\' && line[i + 1]
			&& (line[i + 1] == '$' || line[i + 1] == '\\'
				|| line[i + 1] == '`'))
		{
			if (append_sub(&res, &line[i + 1], 1) < 0)
				return (free(res), NULL);
			i += 2;
			continue ;
		}
		if (line[i] != '$')
		{
			if (append_sub(&res, &line[i], 1) < 0)
				return (free(res), NULL);
			i++;
			continue ;
		}
		i++;
		if (line[i] == '?')
		{
			val = ft_itoa(sh->exit_status);
			if (!val || append_cstr(&res, val) < 0)
				return (free(val), free(res), NULL);
			free(val);
			i++;
			continue ;
		}
		if (line[i] == '{')
		{
			i++;
			n = 0;
			while (line[i + n] && line[i + n] != '}')
				n++;
			name = ft_substr(line, (unsigned int)i, n);
			if (!name)
				return (free(res), NULL);
			val = get_env_value(sh, name);
			free(name);
			name = NULL;
			if (append_cstr(&res, val) < 0)
				return (free(val), free(res), NULL);
			free(val);
			if (line[i + n] == '}')
				i += n + 1;
			else
				i += n;
			continue ;
		}
		n = var_len(&line[i]);
		if (n == 0)
		{
			if (append_sub(&res, "$", 1) < 0)
				return (free(res), NULL);
			continue ;
		}
		name = ft_substr(line, (unsigned int)i, n);
		if (!name)
			return (free(res), NULL);
		val = get_env_value(sh, name);
		free(name);
		name = NULL;
		if (append_cstr(&res, val) < 0)
			return (free(val), free(res), NULL);
		free(val);
		i += n;
	}
	return (res);
}
