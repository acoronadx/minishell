/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_get_word.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 14:25:33 by acoronad          #+#    #+#             */
/*   Updated: 2025/11/06 13:50:59 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* backslash fuera de comillas: consume \x o \⏎ */
static int	handle_bs_outside(const char *s, int *i)
{
	if (s[*i] != '\\')
		return (0);
	if (s[*i + 1] == '\n')
	{
		*i += 2;
		return (1);
	}
	if (s[*i + 1])
	{
		*i += 2;
		return (1);
	}
	*i += 1;
	return (1);
}

/* entrar en comilla simple o doble desde fuera */
static int	enter_quote_outside(const char *s, int *i, t_quote *q)
{
	if (s[*i] == '\'')
	{
		*q = SINGLE_QUOTE;
		*i += 1;
		return (1);
	}
	if (s[*i] == '"')
	{
		*q = DOUBLE_QUOTE;
		*i += 1;
		return (1);
	}
	return (0);
}

/* ¿debemos cortar el token por espacio u operador? */
static int	should_break_outside(const char *s, int i)
{
	if (ft_isspace((unsigned char)s[i]))
		return (1);
	if (is_operator(s + i, NULL, NULL))
		return (1);
	return (0);
}

/* fuera de comillas: gestiona \, comillas y corte por espacio/op */
static int	outside_step(const char *s, int *i, t_quote *q)
{
	if (handle_bs_outside(s, i))
		return (0);
	if (enter_quote_outside(s, i, q))
		return (0);
	if (should_break_outside(s, *i))
		return (1);
	*i += 1;
	return (0);
}


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

/* dentro de comillas dobles: \ escapa $, `, ", \ y newline */
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

/* detecta si el token empezó con comilla para marcar quoted */
static t_quote	detect_qtype(const char *word)
{
	if (word && word[0] == '\'')
		return (SINGLE_QUOTE);
	if (word && word[0] == '"')
		return (DOUBLE_QUOTE);
	return (NO_QUOTE);
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
/* error de comillas sin cerrar: mensaje + liberar lista */
static int	handle_unclosed_quote(t_token **lst)
{
	ft_dprintf(2, "minishell: syntax error: unclosed quote\n");
	free_lexer_list_on_error(lst);
	return (-1);
}

/* extrae el substring del token */
static char	*extract_word(const char *line, int start, int end)
{
	char	*word;

	word = ft_substr(line, start, end - start);
	return (word);
}

/* añade el token T_WORD con quoted correcto; word pasa a la lista o se libera */
static int	append_word_token(t_token **lst, char *word)
{
	t_quote	qtype;

	qtype = detect_qtype(word);
	if (!try_add_token(lst, word, T_WORD, qtype))
	{
		free(word);
		return (-1);
	}
	return (0);
}

int	get_word(const char *line, int i, t_token **lst)
{
	int		start;
	int		end;
	char	*word;

	start = i;
	end = word_end(line, i);
	if (end < 0)
		return (handle_unclosed_quote(lst));
	if (end == start)
		return (end);
	word = extract_word(line, start, end);
	if (!word)
	{
		free_lexer_list_on_error(lst);
		return (-1);
	}
	if (append_word_token(lst, word) < 0)
		return (-1);
	return (end);
}
