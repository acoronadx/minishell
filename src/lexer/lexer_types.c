/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_types.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 16:42:46 by acoronad          #+#    #+#             */
/*   Updated: 2025/06/11 17:02:47 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Detecta operadores de pipe, or, and, background (&), y paréntesis y llaves.
*/
t_token_type	get_pipe_and_or(const char *str, int len)
{
	if (len == 1 && str[0] == '|')
		return (T_PIPE);
	if (len == 2 && str[0] == '|' && str[1] == '|')
		return (T_OR);
	if (len == 2 && str[0] == '&' && str[1] == '&')
		return (T_AND);
	if (len == 1 && str[0] == '&')
		return (T_BG);
	if (len == 1 && str[0] == '(')
		return (T_LPAREN);
	if (len == 1 && str[0] == ')')
		return (T_RPAREN);
	if (len == 1 && str[0] == '{')
		return (T_LBRACE);
	if (len == 1 && str[0] == '}')
		return (T_RBRACE);
	return (T_UNKNOWN);
}

/*
** Detecta ;, = y redirecciones de entrada (<, <<, <<<, <&).
*/
t_token_type	get_semi_redir_left(const char *str, int len)
{
	if (len == 1 && str[0] == ';')
		return (T_SEMI);
	if (len == 1 && str[0] == '=')
		return (T_EQUAL);
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

/*
** Detecta redirecciones de salida (> y variantes).
*/
t_token_type	get_redir_right(const char *str, int len)
{
	if (len == 1 && str[0] == '>')
		return (T_REDIR_OUT);
	if (len == 2 && str[0] == '>' && str[1] == '>')
		return (T_APPEND);
	if (len == 2 && str[0] == '>' && str[1] == '|')
		return (T_FORCE_OUT);
	if (len == 2 && str[0] == '>' && str[1] == '&')
		return (T_DUP_OUT);
	return (T_UNKNOWN);
}

/*
** Detecta redirecciones especiales (&>, &>>, 2>, 2>>).
*/
t_token_type	get_redir_special(const char *str, int len)
{
	if (len == 2 && str[0] == '&' && str[1] == '>')
		return (T_REDIR_ALL);
	if (len == 3 && str[0] == '&' && str[1] == '>' && str[2] == '>')
		return (T_APPEND_ALL);
	if (len == 2 && str[0] == '2' && str[1] == '>')
		return (T_REDIR_ERR);
	if (len == 3 && str[0] == '2' && str[1] == '>' && str[2] == '>')
		return (T_APPEND_ERR);
	return (T_UNKNOWN);
}
