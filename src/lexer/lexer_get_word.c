/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_get_word.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 14:25:33 by acoronad          #+#    #+#             */
/*   Updated: 2025/07/04 11:57:49 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "lexer.h"

/* --- Helper para get_word: calcula final de palabra --- */
static int	word_end(const char *line, int i)
{
	int	escaped;

	escaped = 0;
	while (line[i])
	{
		if (line[i] == '\\' && line[i + 1])
		{
			escaped = 1;
			i += 2;
			continue ;
		}
		if (ft_isspace(line[i]) && !escaped)
			break ;
		if (line[i] == '\'' || line[i] == '"')
			break ;
		if (is_operator(line + i, NULL, NULL))
			break ;
		escaped = 0;
		i++;
	}
	return (i);
}


// Elimina los backslash de escape en una palabra fuera de comillas
char	*remove_backslashes(const char *src)
{
	int		i;
	int		j;
	char	*dst;

	i = 0;
	j = 0;
	dst = malloc(ft_strlen(src) + 1);
	if (!dst)
		return (NULL);
	while (src[i])
	{
		if (src[i] == '\\' && src[i + 1] && src[i + 1] != '$')
			i++;
		dst[j] = src[i];
		if (src[i])
			i++;
		j++;
	}
	dst[j] = '\0';
	return (dst);
}

int	get_word(const char *line, int i, t_token **lst)
{
	int		start;
	int		len;
	char	*word_str;
	char	*unescaped;

	start = i;
	i = word_end(line, i);
	len = i - start;
	if (len == 0)
		return (i);
	word_str = ft_substr(line, start, len);
	if (!word_str)
	{
		free_lexer_list_on_error(lst);
		return (-1);
	}
	unescaped = remove_backslashes(word_str);
	free(word_str);
	if (!unescaped)
	{
		free_lexer_list_on_error(lst);
		return (-1);
	}
	if (try_add_token(lst, unescaped, T_WORD, NO_QUOTE) == 0)
		return (-1);
	return (i);
}
