/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_fork_execve.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 05:11:35 by acoronad          #+#    #+#             */
/*   Updated: 2025/06/17 07:41:22 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"
#include "env.h"

static void	free_envp(char **envp)
{
	int	i;

	i = 0;
	while (envp && envp[i])
	{
		free(envp[i]);
		i++;
	}
	free(envp);
}

static void	child_execve(char **argv, t_shell *shell)
{
	char	**envp;

	envp = env_list_to_envp(shell->env);
	if (!envp)
		exit(127);
	execve(argv[0], argv, envp);
	free_envp(envp);
	ft_dprintf(2, "minishell: %s: command not found\n", argv[0]);
	exit(127);
}

int	fork_execve(char **argv, t_shell *shell)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid < 0)
	{
		ft_dprintf(2, "minishell: fork error\n");
		return (1);
	}
	if (pid == 0)
		child_execve(argv, shell);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		shell->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		shell->exit_status = 128 + WTERMSIG(status);
	return (shell->exit_status);
}
