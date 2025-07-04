/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_ast.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 04:37:31 by acoronad          #+#    #+#             */
/*   Updated: 2025/07/03 10:47:46 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "ast.h"
#include "parser.h"
#include "exec.h"

int	execute_ast(t_ast *node, t_shell *shell)
{
	if (!node)
		return (0);
	if (node->type == N_COMMAND)
		return (execute_command(node, shell));
	else if (node->type == N_PIPE)
		return (execute_pipe(node, shell));
	else if (node->type == N_AND)
		return (execute_and(node, shell));
	else if (node->type == N_OR)
		return (execute_or(node, shell));
	else if (node->type == N_SUBSHELL)
		return (execute_subshell(node, shell));
	else if (node->type == N_SEQUENCE)
		return (execute_sequence(node, shell));
	else if (node->type == N_BACKGROUND)
		return (execute_background(node, shell));
	else if (node->type == N_REDIR)
	{
		ft_dprintf(2, "minishell: syntax error near unexpected token `newline'");
		shell->exit_status = 2;
		return (1);
	}
	// 🔧 Caso por defecto: tipo de nodo desconocido
	ft_dprintf(2, "minishell: command not found\n");
	shell->exit_status = 127;
	return (127);
}
