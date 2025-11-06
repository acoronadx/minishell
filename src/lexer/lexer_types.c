/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_types.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 16:42:46 by acoronad          #+#    #+#             */
/*   Updated: 2025/11/05 15:20:15 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token_type	get_pipe_and_or(const char *str, int len)
{
	if (len == 2 && str[0] == '|' && str[1] == '|')
		return (T_OR);
	if (len == 2 && str[0] == '&' && str[1] == '&')
		return (T_AND);
	if (len == 1 && str[0] == '|')
		return (T_PIPE);
	if (len == 1 && str[0] == '&')
		return (T_BG);
	if (len == 1 && str[0] == ';')
		return (T_SEMI);
	if (len == 1 && str[0] == '(')
		return (T_LPAREN);
	if (len == 1 && str[0] == ')')
		return (T_RPAREN);
	return (T_UNKNOWN);
}

t_token_type	get_semi_redir_left(const char *str, int len)
{
	if (len == 1 && str[0] == '<')
		return (T_REDIR_IN);
	if (len == 2 && str[0] == '<' && str[1] == '<')
		return (T_HEREDOC);
	if (len == 3 && str[0] == '<' && str[1] == '<' && str[2] == '<')
		return (T_HEREDOC_STR);
	if (len == 2 && str[0] == '<' && str[1] == '&')
		return (T_DUP_IN);
	return (T_UNKNOWN);
}

t_token_type	get_redir_right(const char *str, int len)
{
	if (len == 2 && str[0] == '>' && str[1] == '>')
		return (T_APPEND);
	if (len == 2 && str[0] == '>' && str[1] == '|')
		return (T_FORCE_OUT);
	if (len == 2 && str[0] == '>' && str[1] == '&')
		return (T_DUP_OUT);
	if (len == 1 && str[0] == '>')
		return (T_REDIR_OUT);
	return (T_UNKNOWN);
}

t_token_type	get_redir_special(const char *str, int len)
{
	if (len == 3 && str[0] == '&' && str[1] == '>' && str[2] == '>')
		return (T_APPEND_ALL);
	if (len == 2 && str[0] == '&' && str[1] == '>')
		return (T_REDIR_ALL);
	return (T_UNKNOWN);
}
