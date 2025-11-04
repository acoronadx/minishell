/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 14:20:54 by acoronad          #+#    #+#             */
/*   Updated: 2025/11/04 14:04:21 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	restore_std_fds(int saved_in, int saved_out, int saved_err)
{
	if (saved_in != -1)
	{
		dup2(saved_in, STDIN_FILENO);
		close(saved_in);
	}
	if (saved_out != -1)
	{
		dup2(saved_out, STDOUT_FILENO);
		close(saved_out);
	}
	if (saved_err != -1)
	{
		dup2(saved_err, STDERR_FILENO);
		close(saved_err);
	}
}

int	execute_command(t_ast *node, t_shell *shell)
{
	pid_t	pid;
	char	**env;
	char	*exec_path;
	int		status;
	int		saved_in  = -1;
	int		saved_out = -1;
	int		saved_err = -1;

	if (!node)
	{
		shell->exit_status = 0;
		return 0;
	}

	/* === SOLO REDIRECCIONES (sin argv) ===
	   Bash permite comandos vacíos con redirs; aplicamos redirs y devolvemos 0 si no hay error. */
	if ((!node->cmd.argv || !node->cmd.argv[0]) && node->cmd.redirections)
	{
		saved_in  = dup(STDIN_FILENO);
		saved_out = dup(STDOUT_FILENO);
		saved_err = dup(STDERR_FILENO);
		if (saved_in == -1 || saved_out == -1 || saved_err == -1)
		{
			perror("minishell: dup");
			if (saved_in  != -1) close(saved_in);
			if (saved_out != -1) close(saved_out);
			if (saved_err != -1) close(saved_err);
			shell->exit_status = 1;
			return 1;
		}
		if (apply_redirections(node->cmd.redirections) != 0)
		{
			restore_std_fds(saved_in, saved_out, saved_err);
			shell->exit_status = 1;
			return 1;
		}
		restore_std_fds(saved_in, saved_out, saved_err);
		shell->exit_status = 0;
		return 0;
	}

	/* Comando vacío y sin redirecciones → no hace nada, exit 0 */
	if (!node->cmd.argv || !node->cmd.argv[0])
	{
		shell->exit_status = 0;
		return 0;
	}

	/* argv[0] vacío -> "command not found" */
	if (node->cmd.argv[0][0] == '\0')
	{
		ft_dprintf(2, "minishell: : command not found\n");
		shell->exit_status = 127;
		return 127;
	}

	/* Builtins en el proceso padre con redirecciones aplicadas temporalmente */
	if (is_builtin(node->cmd.argv[0]))
	{
		saved_in  = dup(STDIN_FILENO);
		saved_out = dup(STDOUT_FILENO);
		saved_err = dup(STDERR_FILENO);
		if (saved_in == -1 || saved_out == -1 || saved_err == -1)
		{
			perror("minishell: dup");
			if (saved_in  != -1) close(saved_in);
			if (saved_out != -1) close(saved_out);
			if (saved_err != -1) close(saved_err);
			shell->exit_status = 1;
			return 1;
		}
		if (apply_redirections(node->cmd.redirections) != 0)
		{
			restore_std_fds(saved_in, saved_out, saved_err);
			shell->exit_status = 1;
			return 1;
		}
		shell->exit_status = run_builtin(node, shell);
		restore_std_fds(saved_in, saved_out, saved_err);
		return shell->exit_status;
	}

	/* Búsqueda de ejecutable externo */
	exec_path = find_executable(node->cmd.argv[0], shell);
	if (!exec_path)
	{
		if (!ft_strchr(node->cmd.argv[0], '/'))
			return print_exec_error(shell, node->cmd.argv[0], ENOENT);
		if (errno == 0)
			return print_exec_error(shell, node->cmd.argv[0], ENOENT);
		return print_exec_error(shell, node->cmd.argv[0], errno);
	}

	env = env_to_array(shell->env);
	if (!env)
	{
		free(exec_path);
		shell->exit_status = 1;
		return 1;
	}

	pid = fork();
	if (pid < 0)
	{
		status = print_exec_error(shell, node->cmd.argv[0], errno);
		free(exec_path);
		ft_free_strtab(env);
		return status;
	}
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		if (apply_redirections(node->cmd.redirections) != 0)
			_exit(1);
		execve(exec_path, node->cmd.argv, env);
		print_exec_error(shell, node->cmd.argv[0], errno);
		_exit(127);
	}

	if (waitpid(pid, &status, 0) == -1)
		shell->exit_status = 1;
	else if (WIFEXITED(status))
		shell->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		int sig = WTERMSIG(status);
		shell->exit_status = 128 + sig;
		if (sig == SIGINT)
			write(STDOUT_FILENO, "\n", 1);
		else if (sig == SIGQUIT)
			write(STDERR_FILENO, "Quit (core dumped)\n", 19);
	}
	else
		shell->exit_status = 1;

	free(exec_path);
	ft_free_strtab(env);
	return shell->exit_status;
}
