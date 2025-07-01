/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 01:03:45 by acoronad          #+#    #+#             */
/*   Updated: 2025/06/30 01:14:19 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "lexer.h"
#include "parser.h"
#include "ast.h"

int	is_redirection(t_token *tok)
{
	if (!tok)
		return (0);
	return (
		tok->type == T_REDIR_IN
		|| tok->type == T_REDIR_OUT
		|| tok->type == T_APPEND
		|| tok->type == T_HEREDOC
		|| tok->type == T_REDIR_ERR
		|| tok->type == T_APPEND_ERR
		|| tok->type == T_REDIR_ALL
		|| tok->type == T_APPEND_ALL
		|| tok->type == T_FORCE_OUT
		|| tok->type == T_DUP_IN
		|| tok->type == T_DUP_OUT
	);
}

// Convierte t_token_type a t_redir_type
t_redir_type	token_type_to_redir_type(t_token_type type)
{
	if (type == T_REDIR_IN)
		return (REDIR_IN);
	if (type == T_REDIR_OUT)
		return (REDIR_OUT);
	if (type == T_APPEND)
		return (REDIR_APPEND);
	if (type == T_HEREDOC)
		return (REDIR_HEREDOC);
	if (type == T_REDIR_ERR)
		return (REDIR_ERR);
	if (type == T_APPEND_ERR)
		return (REDIR_APPEND_ERR);
	if (type == T_REDIR_ALL)
		return (REDIR_ALL);
	if (type == T_APPEND_ALL)
		return (REDIR_APPEND_ALL);
	if (type == T_FORCE_OUT)
		return (REDIR_FORCE);
	if (type == T_DUP_IN)
		return (REDIR_DUP_IN);
	if (type == T_DUP_OUT)
		return (REDIR_DUP_OUT);
	return (REDIR_INVALID);
}

int	parse_redirections(t_token **cur, t_ast **head, t_ast **tail)
{
	t_ast			*new_redir;
	t_token			*redir_tok;
	t_token			*file_tok;
	t_redir_type	rtype;
	char			*filename_dup;
	int				fd;

	while (*cur && is_redirection(*cur))
	{
		redir_tok = *cur;
		next_token(cur);
		file_tok = *cur;
		if (!file_tok || file_tok->type != T_WORD)
		{
			ft_dprintf(2,
				"minishell: syntax error near unexpected token '%s'\n",
				file_tok ? file_tok->value : "newline");
			return (0);
		}
		rtype = token_type_to_redir_type(redir_tok->type);
		if (rtype == REDIR_INVALID)
			return (0);

		filename_dup = ft_strdup(file_tok->value);
		if (!filename_dup)
			return (0);

		if (rtype == REDIR_HEREDOC)
		{
			fd = heredoc_read(filename_dup);
			if (fd == -1)
			{
				free(filename_dup);
				return (0);
			}
			new_redir = ast_new_redir(filename_dup, NULL, rtype, fd);
		}
		else
		{
			new_redir = ast_new_redir(filename_dup, NULL, rtype, -1);
		}
		if (!new_redir)
		{
			free(filename_dup);
			return (0);
		}
		next_token(cur);
		if (!*head)
		{
			*head = new_redir;
			*tail = new_redir;
		}
		else
		{
			(*tail)->bin.right = new_redir;
			*tail = new_redir;
		}
	}
	return (1);
}
