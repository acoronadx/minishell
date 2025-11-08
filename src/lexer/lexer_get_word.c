/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_get_word.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 14:25:33 by acoronad          #+#    #+#             */
/*   Updated: 2025/11/08 17:08:43 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* dentro de comillas simples: literal hasta encontrar ' */
static void	squote_step(const char *s, int *i, t_quote *q)
{
	if (s[*i] == '\'')
	{
		*q = NO_QUOTE;
		*i += 1;
		return ;
	}
	*i += 1;
}

static void	dquote_step(const char *s, int *i, t_quote *q)
{
	if (s[*i] == '\\' && (s[*i + 1] == '$' || s[*i + 1] == '`'
			|| s[*i + 1] == '"' || s[*i + 1] == '\\'
			|| s[*i + 1] == '\n'))
	{
		if (s[*i + 1])
			*i += 2;
		else
			*i += 1;
		return ;
	}
	if (s[*i] == '"')
	{
		*q = NO_QUOTE;
		*i += 1;
		return ;
	}
	*i += 1;
}

/* calcula fin de palabra o -1 si quedó comilla sin cerrar */
static int	word_end(const char *s, int i)
{
	t_quote	q;

	q = NO_QUOTE;
	while (s[i])
	{
		if (q == NO_QUOTE)
		{
			if (outside_step(s, &i, &q))
				break ;
			continue ;
		}
		if (q == SINGLE_QUOTE)
		{
			squote_step(s, &i, &q);
			continue ;
		}
		dquote_step(s, &i, &q);
	}
	if (q != NO_QUOTE)
		return (-1);
	return (i);
}

static int	handle_unclosed_quote(t_token **lst)
{
	ft_dprintf(2, "minishell: syntax error: unclosed quote\n");
	free_lexer_list_on_error(lst);
	return (-1);
}

int	get_word(const char *line, int i, t_token **lst)
{
	int		start;
	int		end;
	char	*word;
	t_quote	qtype;

	start = i;
	end = word_end(line, i);
	if (end < 0)
		return (handle_unclosed_quote(lst));
	if (end == start)
		return (end);
	word = ft_substr(line, start, end - start);
	if (!word)
		return (free_lexer_list_on_error(lst), -1);
	qtype = detect_qtype(word);
	if (!try_add_token(lst, word, T_WORD, qtype))
		return (free(word), -1);
	return (end);
}
