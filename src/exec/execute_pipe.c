/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 14:11:56 by acoronad          #+#    #+#             */
/*   Updated: 2025/06/30 15:25:31 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"
#include "ast.h"

int	execute_pipe(t_ast *node, t_shell *shell)
{
	int		pipefd[2];
	pid_t	pid_left;
	pid_t	pid_right;
	int		status_left;
	int		status_right;

	if (pipe(pipefd) == -1)
	{
		ft_dprintf(2, "minishell: pipe error\n");
		shell->exit_status = 1;
		return (1);
	}
	pid_left = fork();
	if (pid_left == 0)
	{
		// Proceso hijo izquierdo: escribe en pipe
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
		exit(execute_ast(node->bin.left, shell));
	}
	pid_right = fork();
	if (pid_right == 0)
	{
		// Proceso hijo derecho: lee del pipe
		close(pipefd[1]);
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[0]);
		exit(execute_ast(node->bin.right, shell));
	}
	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(pid_left, &status_left, 0);
	waitpid(pid_right, &status_right, 0);
	if (WIFEXITED(status_right))
		shell->exit_status = WEXITSTATUS(status_right);
	else
		shell->exit_status = 1;
	return (shell->exit_status);
}
