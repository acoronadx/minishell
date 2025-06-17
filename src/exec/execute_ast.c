/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_ast.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 04:37:31 by acoronad          #+#    #+#             */
/*   Updated: 2025/06/17 05:47:41 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_ast(t_ast *node, t_shell *shell)
{
	if (!node)
		return (0);
	if (node->type == N_COMMAND)
		return (execute_command(node, shell));
	if (node->type == N_PIPE)
		return (execute_pipe(node, shell));
	if (node->type == N_SUBSHELL)
		return (execute_subshell(node, shell));
	if (node->type == N_SEQUENCE)
		return (execute_sequence(node, shell));
	if (node->type == N_AND)
		return (execute_and(node, shell));
	if (node->type == N_OR)
		return (execute_or(node, shell));
	if (node->type == N_BACKGROUND)
		return (execute_background(node, shell));
	return (0);
}
