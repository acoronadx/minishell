/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 16:07:05 by acoronad          #+#    #+#             */
/*   Updated: 2025/06/11 16:36:30 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "libft.h"

/*
** Crea un nuevo token y lo añade a la lista.
*/
static void	add_token(t_token **lst, char *str, t_token_type type, t_quote quote)
{
	t_token	*tok;

	tok = token_new(str, type, quote);
	if (tok)
		token_addback(lst, tok);
}

/*
** Extrae y guarda un token palabra desde la posición actual.
*/
static int	get_word(const char *line, int i, t_token **lst)
{
	int		start;
	int		len;

	start = i;
	while (line[i] && line[i] != ' ' && !ft_isspace(line[i]))
	{
		if (line[i] == '\'' || line[i] == '"')
			break ;
		if (is_operator(line + i, NULL, NULL))
			break ;
		i++;
	}
	len = i - start;
	if (len > 0)
		add_token(lst, ft_substr(line, start, len), T_WORD, NO_QUOTE);
	return (i);
}

/*
** Extrae y guarda un token entre comillas simples o dobles.
*/
static int	get_quoted(const char *line, int i, t_token **lst)
{
	char	quote;
	int		start;
	int		len;
	t_quote	qtype;

	quote = line[i];
	qtype = (quote == '\'') ? SINGLE_QUOTE : DOUBLE_QUOTE;
	i++;
	start = i;
	while (line[i] && line[i] != quote)
		i++;
	len = i - start;
	add_token(lst, ft_substr(line, start, len), T_WORD, qtype);
	if (line[i] == quote)
		i++;
	return (i);
}

/*
** Extrae y guarda un token operador (de 1, 2 o 3 caracteres).
*/
static int	get_operator(const char *line, int i, t_token **lst)
{
	t_token_type	type;
	int				len;

	if (is_operator(line + i, &type, &len))
	{
		add_token(lst, ft_substr(line, i, len), type, NO_QUOTE);
		i += len;
	}
	return (i);
}

/*
** Función principal del lexer: tokeniza la línea de entrada.
*/
t_token	*lexer(const char *line)
{
	int		i;
	t_token	*lst;

	i = 0;
	lst = NULL;
	while (line[i])
	{
		while (line[i] && ft_isspace(line[i]))
			i++;
		if (!line[i])
			break ;
		if (line[i] == '\'' || line[i] == '"')
			i = get_quoted(line, i, &lst);
		else if (is_operator(line + i, NULL, NULL))
			i = get_operator(line, i, &lst);
		else
			i = get_word(line, i, &lst);
	}
	return (lst);
}
