/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 14:49:39 by acoronad          #+#    #+#             */
/*   Updated: 2025/11/07 14:44:50 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int set_syntax_err(t_shell *shell)
{
	shell->exit_status = 2;
	return (2);
}

int	shell_exec(t_shell *shell)
{
	shell->tokens = lexer(shell->line);
	if (!shell->tokens)
		return (set_syntax_err(shell));
	expand_variables(shell);
	shell->ast = build_ast(shell->tokens);
	free_token_list(shell->tokens);
	shell->tokens = NULL;
	if (!shell->ast)
		return (set_syntax_err(shell));
	/* Prepara todos los heredocs antes de ejecutar nada */
	if (!prepare_heredocs(shell->ast, shell))
	{
		/* aquí decide: 130 si fue SIGINT en heredoc; 2 si fue sintaxis */
		return (shell->exit_status);
	}

	/* Ejecuta el árbol: cada comando hará su propia expansión previa */
	execute_ast(shell->ast, shell);
	return (shell->exit_status);
}
