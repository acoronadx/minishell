/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_strip_comments_utils.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 12:47:25 by acoronad          #+#    #+#             */
/*   Updated: 2025/11/06 13:04:31 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

/* fuera de comillas: si # comienza palabra, corta la línea */
int	is_comment(char *line, char c, int aws, int i)
{
	if (c == '#' && aws)
	{
		line[i] = '\0';
		return (1);
	}
	return (0);
}

/* fuera de comillas: entra en ' o " */
int	enter_quote_out(char c, t_quote *q, int *i, int *aws)
{
	if (c == '\'')
	{
		*q = SINGLE_QUOTE;
		*i += 1;
		*aws = 0;
		return (1);
	}
	if (c == '"')
	{
		*q = DOUBLE_QUOTE;
		*i += 1;
		*aws = 0;
		return (1);
	}
	return (0);
}

/* desplaza s pos.. fin dos posiciones a la izquierda (borra '\\' y '\n') */
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

/* fuera de comillas: gestiona backslash y continuaciones de línea */
int	handle_bs_out(char *line, int *i, int *aws)
{
	if (line[*i] != '\\')
		return (0);
	if (line[*i + 1] == '\n')
	{
		shift_left_two(line, *i);
		return (1);
	}
	if (line[*i + 1])
		*i += 2;
	else
		*i += 1;
	*aws = 0;
	return (1);
}
