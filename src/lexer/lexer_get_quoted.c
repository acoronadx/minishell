/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_get_quoted.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 14:54:51 by acoronad          #+#    #+#             */
/*   Updated: 2025/06/26 14:58:52 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "lexer.h"

/* --- Helper para get_quoted: calcula final de comillas --- */
static int	quoted_end(const char *line, int i, char quote_char)
{
	while (line[i] && line[i] != quote_char)
		i++;
	return (i);
}

static t_quote	get_quote_type(char c)
{
	if (c == '\'')
		return (SINGLE_QUOTE);
	return (DOUBLE_QUOTE);
}

/* Helper para extraer la subcadena entre comillas y tipo de quote */
static int	get_quoted_str(const char *line, int i, char **quoted_str, t_quote *qtype)
{
	char	quote_char;
	int		start;
	int		len;

	quote_char = line[i];
	*qtype = get_quote_type(quote_char);
	i++;
	start = i;
	i = quoted_end(line, i, quote_char);
	len = i - start;
	*quoted_str = ft_substr(line, start, len);
	return (i);
}

int	get_quoted(const char *line, int i, t_token **lst)
{
	char	*quoted_str;
	t_quote	qtype;

	i = get_quoted_str(line, i, &quoted_str, &qtype);
	if (quoted_str == NULL)
	{
		free_lexer_list_on_error(lst);
		return (-1);
	}
	if (try_add_token(lst, quoted_str, T_WORD, qtype) == 0)
		return (-1);
	if (line[i] && (line[i] == '\'' || line[i] == '"'))
		i++;
	return (i);
}
