/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_heredoc_line.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 17:13:27 by acoronad          #+#    #+#             */
/*   Updated: 2025/11/07 18:17:59 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* ************************************************************************** */
/*  expand_heredoc_line.c                                                     */
/* ************************************************************************** */

/* 
 * Solo expansión de parámetros para here-doc:
 * - Reusa expand_value(line, &k, shell) con k = i+1 cuando vemos '$'
 * - NO hace tilde, NO word splitting, NO quote removal, NO glob
 * - Si expand_value no consume tras '$' (k == i+1), trata '$' como literal
 *   y avanza i solo un carácter.
 */

static size_t hd_calc_len(const char *s, t_shell *sh)
{
	size_t need = 0;
	int    i = 0;

	while (s[i])
	{
		if (s[i] == '$')
		{
			int   k = i + 1;
			char *rep = expand_value(s, &k, sh); /* ya gestiona ${}, $? $$ $0 $VAR, etc. */

			if (!rep)
				return (size_t)-1;
			need += ft_strlen(rep);
			/* Si expand_value no consumió nada (k == i+1), era '$' literal */
			if (k == i + 1)
				i += 1;       /* solo '$' */
			else
				i = k;        /* consumió hasta k */
			free(rep);
			continue;
		}
		need += 1;
		i++;
	}
	return need;
}

char *expand_heredoc_line(const char *line, t_shell *shell)
{
	size_t need;
	char  *out;
	int    i, j;

	if (!line)
		return NULL;

	/* 1ª pasada: calcula longitud */
	need = hd_calc_len(line, shell);
	if (need == (size_t)-1)
		return NULL;

	out = (char *)malloc(need + 1);
	if (!out)
		return NULL;

	/* 2ª pasada: construye salida */
	i = 0;
	j = 0;
	while (line[i])
	{
		if (line[i] == '$')
		{
			int   k = i + 1;
			char *rep = expand_value(line, &k, shell);

			if (!rep)
			{
				free(out);
				return NULL;
			}
			/* Copiamos la expansión (o "$" literal si no consumió nada) */
			{
				size_t n = ft_strlen(rep);
				if (n)
				{
					ft_memcpy(out + j, rep, n);
					j += (int)n;
				}
			}
			/* Avance del índice según si consumió o no */
			if (k == i + 1)
				i += 1;   /* '$' literal */
			else
				i = k;    /* consumió hasta k */
			free(rep);
			continue;
		}
		out[j++] = line[i++];
	}
	out[j] = '\0';
	return out;
}
