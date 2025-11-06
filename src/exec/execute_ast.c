/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_ast.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 04:37:31 by acoronad          #+#    #+#             */
/*   Updated: 2025/10/27 00:48:43 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_ast(t_ast *node, t_shell *shell)
{
	int	ret;

	if (!node)
		return (0);
	if (node->type == N_COMMAND)
		ret = execute_command(node, shell);
	else if (node->type == N_PIPE)
		ret = execute_pipe(node, shell);
	else if (node->type == N_AND)
		ret = execute_and(node, shell);
	else if (node->type == N_OR)
		ret = execute_or(node, shell);
	else if (node->type == N_SUBSHELL)
		ret = execute_subshell(node, shell);
	else if (node->type == N_SEQUENCE)
		ret = execute_sequence(node, shell);
	else if (node->type == N_BACKGROUND)
		ret = execute_background(node, shell);
	else if (node->type == N_REDIR)
	{
		ft_dprintf(2,
			"minishell: syntax error near unexpected token `newline'\n");
		shell->exit_status = 2;
		ret = 1;
	}
	else
	{
		ft_dprintf(2, "minishell: command not found\n");
		shell->exit_status = 127;
		ret = 127;
	}
	return (ret);
}
