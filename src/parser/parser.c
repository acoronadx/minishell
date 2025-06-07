/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 20:56:27 by codespace         #+#    #+#             */
/*   Updated: 2025/06/07 01:35:54 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_token_type	double_char_operator(const char *line, int *len)
{
	if (line[0] == '>' && line[1] == '>')
	{
		*len = 2;
		return (T_REDIR_APPEND);
	}
	if (line[0] == '<' && line[1] == '<')
	{
		*len = 2;
		return (T_HEREDOC);
	}
	if (line[0] == '&' && line[1] == '&')
	{
		*len = 2;
		return (T_AND);
	}
	if (line[0] == '|' && line[1] == '|')
	{
		*len = 2;
		return (T_OR);
	}
	return (T_WORD);
}

static t_token_type	single_char_operator(const char c)
{
	if (c == '|')
		return (T_PIPE);
	if (c == '&')
		return (T_BACKGROUND);
	if (c == ';')
		return (T_SEMICOLON);
	if (c == '<')
		return (T_REDIR_IN);
	if (c == '>')
		return (T_REDIR_OUT);
	if (c == '(')
		return (T_OPEN_PAREN);
	if (c == ')')
		return (T_CLOSE_PAREN);
	return (T_WORD);
}

t_token_type	get_token_type_from_symbol(const char *line, int *len)
{
	t_token_type	type;

	type = double_char_operator(line, len);
	if (*len == 2)
		return (type);
	*len = 1;
	type = single_char_operator(line[0]);
	return (type);
}

void	handle_symbol(t_shell *shell, char **pline)
{
	int				len;
	t_token_type	type;
	char			symbol[3];

	len = 0;
	type = get_token_type_from_symbol(*pline, &len);
	if (len == 2)
	{
		symbol[0] = (*pline)[0];
		symbol[1] = (*pline)[1];
		symbol[2] = '\0';
	}
	else
	{
		symbol[0] = (*pline)[0];
		symbol[1] = '\0';
	}
	ft_add_token(shell, symbol, type, 0, 0);
	*pline += len - 1;
}

static void	handle_quote(t_shell *shell, char **pline)
{
	int		expand;
	char	*token;

	token = ft_strinquotes(*pline, &expand);
	ft_add_token(shell, token, T_WORD, 1, expand);
	*pline += ft_strlen(token) + 2;
	free(token);
	(*pline)--;
}

static void	handle_envvar(t_shell *shell, char **pline)
{
	int	len;

	len = ft_add_envvar(shell, *pline);
	*pline += len - 1;
}

void	parse_line(t_shell *shell, char *line)
{
	while (*line)
	{
		while (ft_isspace(*line))
			line++;
		if (*line == '\0')
			break;
		if (ft_isquote(*line))
			handle_quote(shell, &line);
		else if (is_symbol(*line))
			handle_symbol(shell, &line);
		else if (ft_isenvvar(*line))
			handle_envvar(shell, &line);
		else
			line += ft_add_word(shell, line);
		line++;
	}
}
