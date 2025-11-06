/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 14:11:56 by acoronad          #+#    #+#             */
/*   Updated: 2025/10/27 00:50:18 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_pipe(t_ast *node, t_shell *shell)
{
	int	pipefd[2];
	int	st_l = 0, st_r;
	int	sig;

	pid_t pid_l, pid_r;
	st_l = 0, st_r = 0;
	if (pipe(pipefd) == -1)
	{
		ft_dprintf(2, "minishell: pipe error\n");
		shell->exit_status = 1;
		return (1);
	}
	pid_l = fork();
	if (pid_l == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		close(pipefd[0]);
		if (dup2(pipefd[1], STDOUT_FILENO) < 0)
			_exit(1);
		close(pipefd[1]);
		_exit(execute_ast(node->bin.left, shell) & 0xFF);
	}
	pid_r = fork();
	if (pid_r == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		close(pipefd[1]);
		if (dup2(pipefd[0], STDIN_FILENO) < 0)
			_exit(1);
		close(pipefd[0]);
		_exit(execute_ast(node->bin.right, shell) & 0xFF);
	}
	close(pipefd[0]);
	close(pipefd[1]);
	if (waitpid(pid_l, &st_l, 0) < 0)
		st_l = 0;
	if (waitpid(pid_r, &st_r, 0) < 0)
		st_r = 0;
	if (WIFEXITED(st_r))
		shell->exit_status = WEXITSTATUS(st_r);
	else if (WIFSIGNALED(st_r))
	{
		sig = WTERMSIG(st_r);
		if (sig == SIGINT)
			write(STDOUT_FILENO, "\n", 1);
		else if (sig == SIGQUIT)
			write(STDERR_FILENO, "Quit (core dumped)\n", 19);
		shell->exit_status = 128 + sig;
	}
	else
		shell->exit_status = 1;
	return (shell->exit_status);
}
