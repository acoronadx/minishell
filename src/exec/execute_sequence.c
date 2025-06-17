/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_sequence.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 05:05:58 by acoronad          #+#    #+#             */
/*   Updated: 2025/06/17 05:06:00 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_sequence(t_ast *node, t_shell *shell)
{
	int	status;

	status = 0;
	if (node->left)
		status = execute_ast(node->left, shell);
	if (node->right)
		status = execute_ast(node->right, shell);
	return (status);
}
