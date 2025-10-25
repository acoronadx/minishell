/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_types.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 16:42:46 by acoronad          #+#    #+#             */
/*   Updated: 2025/10/24 14:42:23 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "lexer.h"

t_token_type    get_pipe_and_or(const char *str, int len)
{
    if (len == 2 && str[0] == '|' && str[1] == '|') return (T_OR);
    if (len == 2 && str[0] == '&' && str[1] == '&') return (T_AND);
    if (len == 1 && str[0] == '|') return (T_PIPE);
    if (len == 1 && str[0] == '&') return (T_BG);
    if (len == 1 && str[0] == ';') return (T_SEMI);
    if (len == 1 && str[0] == '(') return (T_LPAREN);
    if (len == 1 && str[0] == ')') return (T_RPAREN);
    if (len == 1 && str[0] == '{') return (T_LBRACE);
    if (len == 1 && str[0] == '}') return (T_RBRACE);
    return (T_UNKNOWN);
}

t_token_type    get_semi_redir_left(const char *str, int len)
{
    if (len == 1 && str[0] == '<') return (T_REDIR_IN);
    if (len == 2 && str[0] == '<' && str[1] == '<') return (T_HEREDOC);
    if (len == 3 && str[0] == '<' && str[1] == '<' && str[2] == '<') return (T_HEREDOC_STR);
    if (len == 2 && str[0] == '<' && str[1] == '&') return (T_DUP_IN);
    if (len == 1 && str[0] == '=') return (T_EQUAL);
    return (T_UNKNOWN);
}

t_token_type    get_redir_right(const char *str, int len)
{
    if (len == 2 && str[0] == '>' && str[1] == '>') return (T_APPEND);
    if (len == 2 && str[0] == '>' && str[1] == '|') return (T_FORCE_OUT);
    if (len == 2 && str[0] == '>' && str[1] == '&') return (T_DUP_OUT);
    if (len == 1 && str[0] == '>') return (T_REDIR_OUT);
    return (T_UNKNOWN);
}

t_token_type    get_redir_special(const char *str, int len)
{
    if (len == 3 && str[0] == '&' && str[1] == '>' && str[2] == '>') return (T_APPEND_ALL);
    if (len == 2 && str[0] == '&' && str[1] == '>') return (T_REDIR_ALL);
    if (len == 3 && str[0] == '2' && str[1] == '>' && str[2] == '>') return (T_APPEND_ERR);
    if (len == 2 && str[0] == '2' && str[1] == '>') return (T_REDIR_ERR);
    return (T_UNKNOWN);
}

t_token_type    get_token_type(const char *str, int len)
{
    t_token_type type;

    type = get_redir_special(str, len);
    if (type != T_UNKNOWN) return type;

    type = get_redir_right(str, len);
    if (type != T_UNKNOWN) return type;

    type = get_semi_redir_left(str, len);
    if (type != T_UNKNOWN) return type;

    type = get_pipe_and_or(str, len);
    if (type != T_UNKNOWN) return type;

    return T_WORD;
}
