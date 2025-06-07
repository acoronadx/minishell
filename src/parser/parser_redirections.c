/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redirections.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 00:51:36 by codespace         #+#    #+#             */
/*   Updated: 2025/06/07 00:52:57 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "errors.h"

static int	is_redir_token(int type)
{
	if (type == T_REDIR_IN || type == T_HEREDOC
		|| type == T_REDIR_OUT || type == T_REDIR_APPEND)
		return (1);
	return (0);
}

static int	handle_redir(t_cmd *cmd, t_token_type t, t_token *token)
{
	if (t == T_REDIR_IN)
		cmd->redir_in = ft_strdup(token->value);
	else if (t == T_HEREDOC)
	{
		cmd->redir_in = ft_strdup(token->value);
		cmd->heredoc = 1;
	}
	else if (t == T_REDIR_OUT)
	{
		cmd->redir_out = ft_strdup(token->value);
		cmd->append_out = 0;
	}
	else if (t == T_REDIR_APPEND)
	{
		cmd->redir_out = ft_strdup(token->value);
		cmd->append_out = 1;
	}
	return (0);
}

int	parse_redirections(t_cmd *cmd, t_token **tokens)
{
	t_token_type	t;
	const char		*unexpected;

	while (*tokens && is_redir_token((*tokens)->type))
	{
		t = (*tokens)->type;
		*tokens = (*tokens)->next;
		if (!(*tokens) || (*tokens)->type != T_WORD)
		{
			unexpected = (*tokens) ? (*tokens)->value : "newline";
			print_syntax_error(unexpected);
			return (1);
		}
		handle_redir(cmd, t, *tokens);
		*tokens = (*tokens)->next;
	}
	return (0);
}
