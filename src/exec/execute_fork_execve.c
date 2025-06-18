/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_fork_execve.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 05:11:35 by acoronad          #+#    #+#             */
/*   Updated: 2025/06/17 20:39:34 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"
#include "env.h"

static void	child_execve(char *path, char **argv, char **envp)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	execve(path, argv, envp);
	free_strtab(envp);
	ft_dprintf(2, "minishell: %s: execution error\n", argv[0]);
	exit(126);
}

static void	handle_child_signal(int status)
{
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
			write(1, "\n", 1);
		else if (WTERMSIG(status) == SIGQUIT)
			write(1, "Quit (core dumped)\n", 19);
	}
}

static int	handle_wait_status(int status, t_shell *shell)
{
	if (WIFEXITED(status))
		shell->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		shell->exit_status = 128 + WTERMSIG(status);
	handle_child_signal(status);
	return (shell->exit_status);
}

int	fork_execve_with_path(char *path, char **argv, t_shell *shell)
{
	pid_t	pid;
	int		status;
	char	**envp;

	envp = env_list_to_envp(shell->env);
	if (!envp)
		return (127);
	restore_vquit(); // <- Antes de fork
	pid = fork();
	if (pid < 0)
	{
		ft_dprintf(2, "minishell: fork error\n");
		free_strtab(envp);
		return (1);
	}
	if (pid == 0)
		child_execve(path, argv, envp); // handler SIGINT/SIGQUIT = SIG_DFL
	free_strtab(envp);
	waitpid(pid, &status, 0);
	return (handle_wait_status(status, shell));
}

