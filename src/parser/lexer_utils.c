/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 20:58:06 by codespace         #+#    #+#             */
/*   Updated: 2025/06/07 01:17:12 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_ispipe(int c)
{
	return (c == '|');
}

int	ft_isenvvar(int c)
{
	return (c == '$');
}

int	ft_isredirection(int c)
{
	return (c == '<' || c == '>');
}

int	ft_isspecial_in_double_quotes(char c)
{
	if (c == '$' || c == '\\' || c == '"')
		return (1);
	return (0);
}

char	*ft_strinquotes(char *str, int *expand)
{
	char	*line;
	char	*start;
	char	quote;

	quote = *str;
	str++;
	start = str;
	while (*str && *str != quote)
		str++;
	line = ft_strldup(start, str - start);

	if (expand)
		*expand = (quote == '"'); // solo expande si es comilla doble

	return (line);
}
