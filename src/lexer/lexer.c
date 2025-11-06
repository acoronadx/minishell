/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 16:07:05 by acoronad          #+#    #+#             */
/*   Updated: 2025/11/05 15:45:16 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* helper: clona la línea y aplica el strip de comentarios bash */
static char	*clone_and_strip(const char *raw)
{
	char	*dup;

	dup = ft_strdup(raw);
	if (!dup)
		return (NULL);
	strip_comment_if_applicable(dup);
	return (dup);
}

/* helper: salta espacios (fuera de comillas) */
static int	skip_spaces(const char *s, int i)
{
	while (s[i] && ft_isspace(s[i]))
		i++;
	return (i);
}

/* helper: lexea un token (operador o palabra) y avanza next_i */
static int	lex_one(const char *line, int i, t_token **lst, int *next_i)
{
	int	ni;

	if (is_operator(line + i, NULL, NULL))
	{
		ni = get_operator(line, i, &*lst);
		if (ni < 0)
			return (-1);
		*next_i = ni;
		return (0);
	}
	ni = get_word(line, i, &*lst);
	if (ni < 0)
		return (-1);
	*next_i = ni;
	return (0);
}

/* paso de bucle: salta espacios, lexea 1 token, avanza índice
 * return:  0 = seguir;  1 = fin;  -1 = error */
static int	lexer_step(char *line, int *i, t_token **lst)
{
	int	next_i;

	*i = skip_spaces(line, *i);
	if (!line[*i])
		return (1);
	if (lex_one(line, *i, lst, &next_i) < 0)
		return (-1);
	*i = next_i;
	return (0);
}

t_token	*lexer(const char *raw_line)
{
	t_token	*lst;
	char	*line;
	int		i;
	int		st;

	lst = NULL;
	if (!raw_line)
		return (NULL);
	line = clone_and_strip(raw_line);
	if (!line)
		return (NULL);
	i = 0;
	while (line[i])
	{
		st = lexer_step(line, &i, &lst);
		if (st < 0)
		{
			free(line);
			return (free_null_token_list(&lst));
		}
		if (st > 0)
			break ;
	}
	free(line);
	return (lst);
}
