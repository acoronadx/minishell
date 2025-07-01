/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_get_word.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 14:25:33 by acoronad          #+#    #+#             */
/*   Updated: 2025/06/30 17:04:42 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "lexer.h"

/* --- Helper para get_word: calcula final de palabra --- */
static int	word_end(const char *line, int i)
{
	while (line[i])
	{
		if (ft_isspace(line[i]))
			break ;
		if (line[i] == '\'' || line[i] == '"')
			break ;
		if (is_operator(line + i, NULL, NULL))
			break ;
		i++;
	}
	return (i);
}

int	get_word(const char *line, int i, t_token **lst)
{
	int		start;
	int		len;
	char	*word_str;

	start = i;
	i = word_end(line, i);
	len = i - start;
	if (len == 0)
		return (i);
	word_str = ft_substr(line, start, len);
	if (word_str == NULL)
	{
		free_lexer_list_on_error(lst);
		return (-1);
	}
	if (try_add_token(lst, word_str, T_WORD, NO_QUOTE) == 0)
		return (-1);
	return (i);
}
