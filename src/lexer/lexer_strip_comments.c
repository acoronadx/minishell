/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_strip_comments.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 13:45:00 by acoronad          #+#    #+#             */
/*   Updated: 2025/10/24 13:45:00 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* fuera de comillas: maneja operadores para reiniciar inicio-de-palabra */
static int	handle_operator_out(const char *line, int *i, int *aws)
{
	t_token_type	t;
	int				len;

	if (is_operator(line + *i, &t, &len))
	{
		*i += len;
		*aws = 1;
		return (1);
	}
	return (0);
}

/* dentro de comillas dobles: gestiona escapes especiales y cierre */
static int	handle_in_dquote(char *line, int *i, t_quote *q, int *aws)
{
	if (line[*i] == '\\' && (line[*i + 1] == '$' || line[*i + 1] == '`'
			|| line[*i + 1] == '"' || line[*i + 1] == '\\'
			|| line[*i + 1] == '\n'))
	{
		if (line[*i + 1] == '\n')
			shift_left_two(line, *i);
		else
			*i += (line[*i + 1] ? 2 : 1);
		*aws = 0;
		return (1);
	}
	if (line[*i] == '"')
	{
		*q = NO_QUOTE;
		*i += 1;
		*aws = 0;
		return (1);
	}
	*i += 1;
	*aws = 0;
	return (1);
}

/* paso fuera de comillas: backslash, comillas, #, operador, espacio, avanzar */
static void	step_outside(char *line, int *i, t_quote *q, int *aws)
{
	char	c;

	c = line[*i];
	if (handle_bs_out(line, i, aws))
		return ;
	if (enter_quote_out(c, q, i, aws))
		return ;
	if (is_comment(line, c, *aws, *i))
		return ;
	if (ft_isspace(c))
	{
		*aws = 1;
		*i += 1;
		return ;
	}
	if (handle_operator_out(line, i, aws))
		return ;
	*aws = 0;
	*i += 1;
}

/* paso en comillas simples: todo literal hasta encontrar ' */
static void	step_in_squote(const char *line, int *i, t_quote *q, int *aws)
{
	if (line[*i] == '\'')
	{
		*q = NO_QUOTE;
		*i += 1;
		*aws = 0;
		return ;
	}
	*i += 1;
	*aws = 0;
}

void	strip_comment_if_applicable(char *line)
{
	int		i;
	int		at_word_start;
	t_quote	q;

	if (!line)
		return ;
	i = 0;
	q = NO_QUOTE;
	at_word_start = 1;
	while (line[i])
	{
		if (q == NO_QUOTE)
			step_outside(line, &i, &q, &at_word_start);
		else if (q == SINGLE_QUOTE)
			step_in_squote(line, &i, &q, &at_word_start);
		else
			handle_in_dquote(line, &i, &q, &at_word_start);
		if (!line[i])
			break ;
	}
}
