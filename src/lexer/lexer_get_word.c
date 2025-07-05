/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_get_word.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 14:25:33 by acoronad          #+#    #+#             */
/*   Updated: 2025/07/05 16:21:44 by acoronad         ###   ########.fr       */
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
/*		if ((line[i] == '\'' && line[i + 1] == '\'') || (line[i] == '"' && line[i + 1] == '"'))
		{
			if (line[i + 2] && ft_isspace(line[i + 2]))
				return (i);
			i += 2;
			continue ;
		}			
*/
		if (ft_isspace(line[i]) && !escaped)
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
char *remove_quotes_inside_double_quotes(const char *src)
{
    char *dst;
    int i = 0, j = 0;
    int in_dquote = 0;

    dst = malloc(ft_strlen(src) + 1);
    if (!dst)
        return NULL;

    while (src[i])
    {
        if (src[i] == '"')
        {
            in_dquote = !in_dquote;
            i++;
            continue;
        }
        if (src[i] == '\'' && in_dquote)
        {
            // ignora comilla simple dentro de comillas dobles
            i++;
            continue;
        }
        dst[j++] = src[i++];
    }
    dst[j] = '\0';
    return dst;
}


char	*save_expander_operator(char *src, t_token **lst)
{
	int		i;
	int		start;
	char	*before_quotes;
	char	*after_quotes;
	char	*joined;

	i = 1;
	start = 1;
	// Recorre el nombre de variable: letras, números, guiones bajos
	while (src[i] && (ft_isalnum(src[i]) || src[i] == '_'))
		i++;
	// Si no hay nombre de variable y lo que viene son comillas vacías, $"" o $'', devuelve string vacía
	if (i == 1 && ((src[i] == '"' && src[i + 1] == '"')
			|| (src[i] == '\'' && src[i + 1] == '\'')))
		return (ft_strdup(""));

	// Caso base: solo la variable, sin comillas vacías
	if (!src[i])
		return (ft_strdup(src));

	// Si hay comillas vacías justo después de variable, las saltamos
	if ((src[i] == '"' && src[i + 1] == '"')
		|| (src[i] == '\'' && src[i + 1] == '\''))
		i += 2;

	// Construye el resultado
	before_quotes = ft_substr(src, 0, start = i);
	if (!before_quotes)
		return (free_lexer_list_on_error(lst), NULL);
	after_quotes = ft_strdup(src + i);
	if (!after_quotes)
		return (free(before_quotes), free_lexer_list_on_error(lst), NULL);
	joined = ft_strjoin(before_quotes, after_quotes);
	free(before_quotes);
	free(after_quotes);
	return (joined);
}


int	get_word(const char *line, int i, t_token **lst)
{
	int		start;
	int		len;
	char	*word_str;
	char	*unescaped;
	char	*cleaned;

	start = i;
	i = word_end(line, i);
	len = i - start;
	if (len == 0)
		return (i);
	word_str = ft_substr(line, start, len);
	if (!word_str)
		return (free_lexer_list_on_error(lst), -1);
	unescaped = remove_backslashes(word_str);
	free(word_str);
	if (!unescaped)
		return (free_lexer_list_on_error(lst), -1);
	if (unescaped[0] == '$')
		cleaned = save_expander_operator(unescaped, lst);
	else
		cleaned = remove_quotes_inside_double_quotes(unescaped);
	free(unescaped);
	if (!cleaned)
		return (free_lexer_list_on_error(lst), -1);
	if (try_add_token(lst, cleaned, T_WORD, NO_QUOTE) == 0)
		return (-1);
	return (i);
}
