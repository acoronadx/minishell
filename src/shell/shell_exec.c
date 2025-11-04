/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 14:49:39 by acoronad          #+#    #+#             */
/*   Updated: 2025/11/04 14:40:54 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	shell_exec(t_shell *shell)
{
	shell->tokens = lexer(shell->line);
	if (!shell->tokens)
	{
		shell->exit_status = 2; // Error de sintaxis como Bash
		return (2);
	}
	expand_variables(shell);
	shell->ast = build_ast(shell->tokens);
	free_token_list(shell->tokens);
	shell->tokens = NULL;
	if (!shell->ast)
	{
		shell->exit_status = 2; // Error de sintaxis como Bash
		return (2);
	}
	execute_ast(shell->ast, shell);
	return (shell->exit_status);
}
