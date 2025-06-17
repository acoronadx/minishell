/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_background.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 05:07:27 by acoronad          #+#    #+#             */
/*   Updated: 2025/06/17 07:53:07 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"
#include "ast.h"

int	execute_background(t_ast *node, t_shell *shell)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		execute_ast(node->left, shell);
		exit(0);
	}
	if (node->right)
		return (execute_ast(node->right, shell));
	return (0);
}
