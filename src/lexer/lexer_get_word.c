/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_get_word.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 14:25:33 by acoronad          #+#    #+#             */
/*   Updated: 2025/11/01 15:08:40 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "lexer.h"

static int	word_end(const char *s, int i)
{
	t_quote	q;

	q = NO_QUOTE;
	while (s[i])
	{
		if (q == NO_QUOTE)
		{
			if (s[i] == '\\')
			{
				if (s[i + 1] == '\n')
				{
					i += 2;
					continue ;
				}
				if (s[i + 1])
					i += 2;
				else
					i += 1;
				continue ;
			}
			if (s[i] == '\'')
			{
				q = SINGLE_QUOTE;
				i++;
				continue ;
			}
			if (s[i] == '"')
			{
				q = DOUBLE_QUOTE;
				i++;
				continue ;
			}
			if (ft_isspace(s[i]))
				break ;
			if (is_operator(s + i, NULL, NULL) && s[i] != '=')
				break;
			i++;
			continue ;
		}
		if (q == SINGLE_QUOTE)
		{
			if (s[i] == '\'')
				q = NO_QUOTE;
			i++;
			continue ;
		}
		if (s[i] == '\\' && (s[i + 1] == '$' || s[i + 1] == '`'
				|| s[i + 1] == '"' || s[i + 1] == '\\'
				|| s[i + 1] == '\n'))
		{
			if (s[i + 1])
				i += 2;
			else
				i += 1;
			continue ;
		}
		if (s[i] == '"')
			q = NO_QUOTE;
		i++;
	}
	if (q != NO_QUOTE)
		return (-1);
	return (i);
}

int	get_word(const char *line, int i, t_token **lst)
{
	int		start;
	int		end;
	char	*word;
	t_quote	qtype;

	start = i;
	end = word_end(line, i);
	qtype = NO_QUOTE;
	if (end < 0)
	{
		fprintf(stderr, "minishell: syntax error: unclosed quote\n");
		free_lexer_list_on_error(lst);
		return (-1);
	}
	if (end == start)
		return (end);
	word = ft_substr(line, start, end - start);
	if (!word)
	{
		free_lexer_list_on_error(lst);
		return (-1);
	}
	if (word[0] == '\'')
		qtype = SINGLE_QUOTE;
	else if (word[0] == '"')
		qtype = DOUBLE_QUOTE;
	if (!try_add_token(lst, word, T_WORD, qtype))
	{
		free(word);
		return (-1);
	}
	return (end);
}
