/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_and_or.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 05:06:26 by acoronad          #+#    #+#             */
/*   Updated: 2025/06/17 07:35:19 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"

int	execute_and(t_ast *node, t_shell *shell)
{
	int	status;

	status = 0;
	if (node->left)
		status = execute_ast(node->left, shell);
	if (status == 0 && node->right)
		status = execute_ast(node->right, shell);
	return (status);
}

int	execute_or(t_ast *node, t_shell *shell)
{
	int	status;

	status = 0;
	if (node->left)
		status = execute_ast(node->left, shell);
	if (status != 0 && node->right)
		status = execute_ast(node->right, shell);
	return (status);
}
