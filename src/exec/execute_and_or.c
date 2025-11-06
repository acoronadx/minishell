/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_and_or.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 14:12:48 by acoronad          #+#    #+#             */
/*   Updated: 2025/10/27 00:49:04 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_and(t_ast *node, t_shell *shell)
{
	int	ret;

	ret = execute_ast(node->bin.left, shell);
	if (ret == 0)
		ret = execute_ast(node->bin.right, shell);
	else
		shell->exit_status = ret;
	return (ret);
}

int	execute_or(t_ast *node, t_shell *shell)
{
	int	ret;

	ret = execute_ast(node->bin.left, shell);
	if (ret != 0)
		ret = execute_ast(node->bin.right, shell);
	else
		shell->exit_status = ret;
	return (ret);
}
