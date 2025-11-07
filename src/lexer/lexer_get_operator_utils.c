/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_get_operator_utils.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 15:41:21 by acoronad          #+#    #+#             */
/*   Updated: 2025/11/06 16:23:28 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	op_set(t_token_type *type, int *len, t_token_type t, int l)
{
	if (type)
		*type = t;
	if (len)
		*len = l;
}

static int	op_match_len3(const char *s, t_token_type *type, int *len)
{
	if (!s)
		return (0);
	if (ft_strncmp(s, "<<<", 3) == 0)
		return (op_set(type, len, T_HEREDOC_STR, 3), 1);
	if (ft_strncmp(s, "2>>", 3) == 0)
		return (op_set(type, len, T_APPEND_ERR, 3), 1);
	if (ft_strncmp(s, "&>>", 3) == 0)
		return (op_set(type, len, T_APPEND_ALL, 3), 1);
	return (0);
}

static int	op_match_len2(const char *s, t_token_type *type, int *len)
{
	if (!s)
		return (0);
	if (ft_strncmp(s, ">>", 2) == 0)
		return (op_set(type, len, T_APPEND, 2), 1);
	if (ft_strncmp(s, "<<", 2) == 0)
		return (op_set(type, len, T_HEREDOC, 2), 1);
	if (ft_strncmp(s, "||", 2) == 0)
		return (op_set(type, len, T_OR, 2), 1);
	if (ft_strncmp(s, "&&", 2) == 0)
		return (op_set(type, len, T_AND, 2), 1);
	if (ft_strncmp(s, "&>", 2) == 0)
		return (op_set(type, len, T_REDIR_ALL, 2), 1);
	if (ft_strncmp(s, ">|", 2) == 0)
		return (op_set(type, len, T_FORCE_OUT, 2), 1);
	if (ft_strncmp(s, "<&", 2) == 0)
		return (op_set(type, len, T_DUP_IN, 2), 1);
	if (ft_strncmp(s, ">&", 2) == 0)
		return (op_set(type, len, T_DUP_OUT, 2), 1);
	if (ft_strncmp(s, "2>", 2) == 0)
		return (op_set(type, len, T_REDIR_ERR, 2), 1);
	return (0);
}

static int	op_match_len1(const char *s, t_token_type *type, int *len)
{
	if (!s)
		return (0);
	if (*s == '|') return (op_set(type, len, T_PIPE, 1), 1);
	if (*s == '&') return (op_set(type, len, T_BG, 1), 1);
	if (*s == ';') return (op_set(type, len, T_SEMI, 1), 1);
	if (*s == '<') return (op_set(type, len, T_REDIR_IN, 1), 1);
	if (*s == '>') return (op_set(type, len, T_REDIR_OUT, 1), 1);
	if (*s == '(') return (op_set(type, len, T_LPAREN, 1), 1);
	if (*s == ')') return (op_set(type, len, T_RPAREN, 1), 1);
	return (0);
}

int	is_operator(const char *str, t_token_type *type, int *len)
{
	if (!str)
		return (0);
	if (op_match_len3(str, type, len))
		return (1);
	if (op_match_len2(str, type, len))
		return (1);
	if (op_match_len1(str, type, len))
		return (1);
	return (0);
}
