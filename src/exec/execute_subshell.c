/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_subshell.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 14:13:14 by acoronad          #+#    #+#             */
/*   Updated: 2025/06/29 14:13:24 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"
#include "ast.h"

int execute_subshell(t_ast *node, t_shell *shell)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid < 0)
	{
		ft_dprintf(2, "minishell: fork error\n");
		shell->exit_status = 1;
		return (1);
	}
	if (pid == 0)
	{
		execute_ast(node->subshell.child, shell);
		exit(shell->exit_status);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		shell->exit_status = WEXITSTATUS(status);
	else
		shell->exit_status = 1;
	return (shell->exit_status);
}
