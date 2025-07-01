/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command_and_redirection.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 06:56:48 by acoronad          #+#    #+#             */
/*   Updated: 2025/06/29 14:16:27 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include "ast.h"
#include "lexer.h"

t_ast	*parse_command_and_redirections(t_token **cur)
{
	t_ast	*cmd_node;
	t_ast	*redir_head;
	t_ast	*redir_tail;

	cmd_node = NULL;
	redir_head = NULL;
	redir_tail = NULL;
	if (!parse_redirections(cur, &redir_head, &redir_tail))
		return (NULL);
	if (*cur && (*cur)->type == T_LPAREN)
		cmd_node = parse_subshell(cur);
	else if (*cur && (*cur)->type == T_WORD)
		cmd_node = parse_simple_command(cur);
	else
	{
		if (!redir_head)
		{
			ft_dprintf(2,
				"minishell: syntax error near unexpected token '%s'\n",
				*cur ? (*cur)->value : "newline");
			return (NULL);
		}
		ft_dprintf(2,
			"minishell: syntax error: command expected after redirections\n");
		free_ast(redir_head);
		return (NULL);
	}
	if (!cmd_node)
	{
		free_ast(redir_head);
		return (NULL);
	}
	if (!parse_redirections(cur, &redir_head, &redir_tail))
	{
		free_ast(cmd_node);
		free_ast(redir_head);
		return (NULL);
	}
	if (redir_head)
		cmd_node->cmd.redirections = redir_head;
	return (cmd_node);
}
