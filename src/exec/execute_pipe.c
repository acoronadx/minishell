/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 05:08:40 by acoronad          #+#    #+#             */
/*   Updated: 2025/06/17 05:51:52 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	close_pipe(int *fd)
{
	close(fd[0]);
	close(fd[1]);
}

static int	child_left(t_ast *node, t_shell *shell, int *pipe_fd)
{
	if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
		return (1);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	exit(execute_ast(node->left, shell));
	return (0);
}

static int	child_right(t_ast *node, t_shell *shell, int *pipe_fd)
{
	if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
		return (1);
	close(pipe_fd[1]);
	close(pipe_fd[0]);
	exit(execute_ast(node->right, shell));
	return (0);
}

static int	wait_both(pid_t pid1, pid_t pid2)
{
	int	status;

	waitpid(pid1, NULL, 0);
	waitpid(pid2, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}

int	execute_pipe(t_ast *node, t_shell *shell)
{
	int		pipe_fd[2];
	pid_t	pid1;
	pid_t	pid2;

	if (pipe(pipe_fd) == -1)
		return (1);
	pid1 = fork();
	if (pid1 == -1)
	{
		close_pipe(pipe_fd);
		return (1);
	}
	if (pid1 == 0)
		child_left(node, shell, pipe_fd);
	pid2 = fork();
	if (pid2 == -1)
	{
		close_pipe(pipe_fd);
		return (1);
	}
	if (pid2 == 0)
		child_right(node, shell, pipe_fd);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	return (wait_both(pid1, pid2));
}
