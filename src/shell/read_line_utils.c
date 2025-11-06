/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_line_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 17:20:21 by acoronad          #+#    #+#             */
/*   Updated: 2025/11/04 17:20:32 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Estado final de comillas: 0, '\'' o '"' */
static int	quote_state(const char *s)
{
	int	i;
	int	qt;

	if (!s)
		return (0);
	qt = 0;
	i = 0;
	while (s[i])
	{
		if (!qt && (s[i] == '\'' || s[i] == '"'))
			qt = s[i];
		else if ((qt == '\'' && s[i] == '\'') || (qt == '"' && s[i] == '"'))
			qt = 0;
		i++;
	}
	return (qt);
}

/* número de backslashes finales */
static int	trailing_bslashes(const char *s)
{
	int	i;
	int	n;

	if (!s)
		return (0);
	i = (int)ft_strlen(s) - 1;
	n = 0;
	while (i >= 0 && s[i] == '\\')
	{
		n++;
		i--;
	}
	return (n);
}

/* público: backslash de continuación cerrado (1 = cerrado) */
int	ft_backslash_closed(const char *s)
{
	int	qt;

	if (!s)
		return (1);
	qt = quote_state(s);
	if (qt == '\'')
		return (1);
	return ((trailing_bslashes(s) & 1) == 0);
}

/* añade "\n" + next al acumulador; devuelve 0 ok / -1 error */
int	append_next_line(char **accum, const char *prompt)
{
	char	*next;
	char	*tmp;

	setup_prompt_signals();
	next = readline(prompt);
	if (!next)
		return (-1);
	tmp = ft_strjoin(*accum ? *accum : "", "\n");
	if (!tmp)
		return (free(next), -1);
	free(*accum);
	*accum = ft_strjoin(tmp, next);
	free(tmp);
	free(next);
	return (*accum ? 0 : -1);
}
