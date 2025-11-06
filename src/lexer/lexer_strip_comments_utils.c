/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_strip_comments_utils.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 12:47:25 by acoronad          #+#    #+#             */
/*   Updated: 2025/11/06 14:29:58 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	shift_left_two(char *s, int pos)
{
	int	j;

	j = pos;
	while (s[j + 2])
	{
		s[j] = s[j + 2];
		j++;
	}
	s[j] = '\0';
}

int	handle_bs_out(char *line, int *i, int *aws)
{
	if (line[*i] != '\\')
		return (0);
	if (line[*i + 1] == '\n')
		shift_left_two(line, *i);
	else if (line[*i + 1])
		*i += 2;
	else
		*i += 1;
	*aws = 0;
	return (1);
}

int	enter_quote_out(char c, t_quote *q, int *i, int *aws)
{
	if (c == '\'')
		return (*q = SINGLE_QUOTE, *i += 1, *aws = 0, 1);
	if (c == '"')
		return (*q = DOUBLE_QUOTE, *i += 1, *aws = 0, 1);
	return (0);
}

int	is_comment(char *line, char c, int aws, int i)
{
	if (c == '#' && aws)
	{
		line[i] = '\0';
		return (1);
	}
	return (0);
}

int	handle_operator_out(const char *line, int *i, int *aws)
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
