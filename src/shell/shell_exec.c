/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 14:49:39 by acoronad          #+#    #+#             */
/*   Updated: 2025/11/07 23:29:59 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	set_syntax_err(t_shell *shell)
{
	shell->exit_status = 2;
	return (2);
}

int	shell_exec(t_shell *shell)
{
	shell->tokens = lexer(shell->line);
	if (!shell->tokens)
		return (set_syntax_err(shell));
	mark_heredoc_delims(shell->tokens);
	expand_variables(shell);
	shell->ast = build_ast(shell->tokens);
	free_token_list(shell->tokens);
	shell->tokens = NULL;
	if (!shell->ast)
		return (set_syntax_err(shell));
	if (prepare_heredocs(shell->ast, shell) != 0)
		return (shell->exit_status);
	execute_ast(shell->ast, shell);
	return (shell->exit_status);
}
