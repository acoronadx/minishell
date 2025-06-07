/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 20:52:39 by codespace         #+#    #+#             */
/*   Updated: 2025/06/07 01:27:55 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_add_token(t_shell *shell, char *value, t_token_type type,
	int in_quotes, int expand)
{
	t_token	*new;
	t_token	*last;

	new = malloc(sizeof(t_token));
	if (!new)
	return ;
	new->value = ft_strdup(value);
	new->type = type;
	new->in_quotes = in_quotes;
	new->expand = expand;
	new->next = NULL;

	if (!shell->tokens)
	shell->tokens = new;
	else
	{
	last = shell->tokens;
	while (last->next)
	last = last->next;
	last->next = new;
	}
}

int	ft_add_redirection(t_shell *shell, char *line)
{
	if (*line == '<')
	{
		if (line[1] && line[1] == '<')
		{
			ft_add_token(shell, "<<", T_HEREDOC, 0, 0);
			return (1); // ← se suma solo 1 porque parse_line hace line++
		}
		else
			ft_add_token(shell, "<", T_REDIR_IN, 0, 0);
	}
	else if (*line == '>')
	{
		if (line[1] && line[1] == '>')
		{
			ft_add_token(shell, ">>", T_REDIR_APPEND, 0, 0);
			return (1);
		}
		else
			ft_add_token(shell, ">", T_REDIR_OUT, 0, 0);
	}
	return (0);
}

int	ft_add_word(t_shell *shell, char *line)
{
	int		len = 0;
	char	*word;

	while (line[len] && !ft_issymbol)
		len++;
	if (len == 0)
		return (0);
	word = ft_strldup(line, len);
	if (!word)
		return (0);
	ft_add_token(shell, word, T_WORD, 0, 1); // palabras sin comillas → expandibles
	free(word);
	return (len);
}
