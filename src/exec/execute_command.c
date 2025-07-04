/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 14:20:54 by acoronad          #+#    #+#             */
/*   Updated: 2025/07/03 19:09:27 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "exec.h"
#include "ast.h"
#include "builtins.h"

int	execute_command(t_ast *node, t_shell *shell)
{
	pid_t	pid;
	char	**env;
	char	*exec_path;
	int		ret;

	if (!node || !node->cmd.argv || !node->cmd.argv[0])
		return (0);

	if (node->cmd.argv[0][0] == '\0')
	{
		ft_dprintf(2, "minishell: : command not found\n");
		shell->exit_status = 127;
		return (127);
	}
	if (g_signal)
	{
		shell->exit_status = 130;
		g_signal = 0;
		return (130);
	}
	if (is_builtin(node->cmd.argv[0]))
	{
		shell->exit_status = run_builtin(node, shell);
		return (shell->exit_status);
	}
	exec_path = find_executable(node->cmd.argv[0], shell);
	if (!exec_path)
		return (print_exec_error(shell, node->cmd.argv[0], errno));
	env = env_to_array(shell->env);
	if (!env)
	{
		free(exec_path);
		return (1);
	}
	pid = fork();
	if (pid < 0)
	{
		free(exec_path);
		ft_free_strtab(env);
		return (print_exec_error(shell, node->cmd.argv[0], errno));
	}
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		ret = apply_redirections(node->cmd.redirections);
		if (ret != 0)
			exit(1);
		execve(exec_path, node->cmd.argv, env);
		print_exec_error(shell, node->cmd.argv[0], errno);
		exit(127);
	}
	waitpid(pid, &shell->exit_status, 0);
	if (WIFEXITED(shell->exit_status))
		shell->exit_status = WEXITSTATUS(shell->exit_status);
	else if (WIFSIGNALED(shell->exit_status))
		shell->exit_status = 128 + WTERMSIG(shell->exit_status);
	free(exec_path);
	ft_free_strtab(env);
	return (shell->exit_status);
}
