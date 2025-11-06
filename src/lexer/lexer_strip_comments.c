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

void	strip_comment_if_applicable(char *line)
{
	int					i;
	t_quote				q;
	char				c;
	int					j;
	t_token_type		t;
	int					len;

	i = 0;
	q = NO_QUOTE;
	int at_word_start = 1;
		/* al principio de la línea estamos al inicio de palabra */
	if (!line)
		return ;
	while (line[i])
	{
		c = line[i];
		/* Gestión de escapes fuera/dentro de comillas según bash */
		if (q == NO_QUOTE)
		{
			if (c == '\\')
			{
				if (line[i + 1] == '\n')
				{
					/* continuación de línea: elimina ambos */
					/* desplazamos resto hacia atrás */
					j = i;
					while (line[j + 2])
					{
						line[j] = line[j + 2];
						j++;
					}
					line[j] = '\0';
					continue ;
				}
				/* salta el próximo carácter como literal */
				if (line[i + 1])
					i += 2;
				else
					i++;
				at_word_start = 0;
				continue ;
			}
			if (c == '\'')
			{
				q = SINGLE_QUOTE;
				i++;
				at_word_start = 0;
				continue ;
			}
			if (c == '"')
			{
				q = DOUBLE_QUOTE;
				i++;
				at_word_start = 0;
				continue ;
			}
			/* Si # comienza palabra y estamos fuera de comillas -> cortar */
			if (c == '#' && at_word_start)
			{
				line[i] = '\0';
				return ;
			}
			/* espacios reinician inicio-de-palabra */
			if (ft_isspace(c))
				at_word_start = 1;
			else
			{
				/* si el carácter actual inicia un operador,
					el siguiente es inicio-de-palabra */
				if (is_operator(line + i, &t, &len))
				{
					i += len;
					at_word_start = 1;
					continue ;
				}
				at_word_start = 0;
			}
			i++;
		}
		else if (q == SINGLE_QUOTE)
		{
			/* dentro de comillas simples: todo literal hasta la siguiente ' */
			if (c == '\'')
				q = NO_QUOTE;
			i++;
			at_word_start = 0;
		}
		else /* DOUBLE_QUOTE */
		{
			/* Dentro de dobles comillas, \ sólo escapa $, `, ",
				\ y newline. :contentReference[oaicite:7]{index=7} */
			if (c == '\\' && (line[i + 1] == '$' || line[i + 1] == '`' || line[i
					+ 1] == '"' || line[i + 1] == '\\' || line[i + 1] == '\n'))
			{
				if (line[i + 1] == '\n')
				{
					/* quita la continuación de línea */
					j = i;
					while (line[j + 2])
					{
						line[j] = line[j + 2];
						j++;
					}
					line[j] = '\0';
					continue ;
				}
				i += (line[i + 1] ? 2 : 1);
				at_word_start = 0;
				continue ;
			}
			if (c == '"')
				q = NO_QUOTE;
			i++;
			at_word_start = 0;
		}
	}
}
