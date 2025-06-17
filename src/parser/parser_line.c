/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_line.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 14:51:59 by acoronad          #+#    #+#             */
/*   Updated: 2025/06/17 08:38:07 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include "lexer.h"
#include "ast.h"

/*
** Parses the raw input line into an abstract syntax tree (AST).
** Returns a pointer to the root node or NULL on error.
*/

t_ast	*parser_line(t_shell *shell)
{
	shell->tokens = lexer(shell->line);
	if (!shell->tokens)
		return (NULL);
	shell->ast = build_ast(shell->tokens);
	free_token_list(shell->tokens);
	shell->tokens = NULL;
	if (!shell->ast)
	{
		ft_dprintf(2, "minishell: syntax error near unexpected token\n");
		return (NULL);
	}
	return (shell->ast);
}
