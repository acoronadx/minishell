/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_get_word_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 14:31:12 by acoronad          #+#    #+#             */
/*   Updated: 2025/11/06 14:31:24 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_bs_outside(const char *s, int *i)
{
	if (s[*i] != '\\')
		return (0);
	if (s[*i + 1] == '\n')
		*i += 2;
	else if (s[*i + 1])
		*i += 2;
	else
		*i += 1;
	return (1);
}

static int	enter_quote_outside(const char *s, int *i, t_quote *q)
{
	if (s[*i] == '\'')
		return (*q = SINGLE_QUOTE, *i += 1, 1);
	if (s[*i] == '"')
		return (*q = DOUBLE_QUOTE, *i += 1, 1);
	return (0);
}

static int	should_break_outside(const char *s, int i)
{
	if (ft_isspace((unsigned char)s[i]))
		return (1);
	if (is_operator(s + i, NULL, NULL))
		return (1);
	return (0);
}

int	outside_step(const char *s, int *i, t_quote *q)
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

t_quote	detect_qtype(const char *word)
{
	if (word && word[0] == '\'')
		return (SINGLE_QUOTE);
	if (word && word[0] == '"')
		return (DOUBLE_QUOTE);
	return (NO_QUOTE);
}
