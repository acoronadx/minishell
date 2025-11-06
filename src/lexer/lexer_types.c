/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_types.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 14:28:57 by acoronad          #+#    #+#             */
/*   Updated: 2025/11/06 14:28:59 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_token_type	get_redir_special(const char *s, int len)
{
	if (len == 3 && s[0] == '&' && s[1] == '>' && s[2] == '>')
		return (T_APPEND_ALL);
	if (len == 2 && s[0] == '&' && s[1] == '>')
		return (T_REDIR_ALL);
	return (T_UNKNOWN);
}

static t_token_type	get_redir_right(const char *s, int len)
{
	if (len == 2 && s[0] == '>' && s[1] == '>')
		return (T_APPEND);
	if (len == 2 && s[0] == '>' && s[1] == '|')
		return (T_FORCE_OUT);
	if (len == 2 && s[0] == '>' && s[1] == '&')
		return (T_DUP_OUT);
	if (len == 1 && s[0] == '>')
		return (T_REDIR_OUT);
	return (T_UNKNOWN);
}

static t_token_type	get_semi_redir_left(const char *s, int len)
{
	if (len == 1 && s[0] == '<')
		return (T_REDIR_IN);
	if (len == 2 && s[0] == '<' && s[1] == '<')
		return (T_HEREDOC);
	if (len == 3 && s[0] == '<' && s[1] == '<' && s[2] == '<')
		return (T_HEREDOC_STR);
	if (len == 2 && s[0] == '<' && s[1] == '&')
		return (T_DUP_IN);
	return (T_UNKNOWN);
}

static t_token_type	get_pipe_and_or(const char *s, int len)
{
	if (len == 2 && s[0] == '|' && s[1] == '|')
		return (T_OR);
	if (len == 2 && s[0] == '&' && s[1] == '&')
		return (T_AND);
	if (len == 1 && s[0] == '|')
		return (T_PIPE);
	if (len == 1 && s[0] == '&')
		return (T_BG);
	if (len == 1 && s[0] == ';')
		return (T_SEMI);
	return (T_UNKNOWN);
}

t_token_type	get_token_type(const char *str, int len)
{
	t_token_type	t;

	t = get_redir_special(str, len);
	if (t != T_UNKNOWN)
		return (t);
	t = get_redir_right(str, len);
	if (t != T_UNKNOWN)
		return (t);
	t = get_semi_redir_left(str, len);
	if (t != T_UNKNOWN)
		return (t);
	t = get_pipe_and_or(str, len);
	if (t != T_UNKNOWN)
		return (t);
	return (T_WORD);
}
