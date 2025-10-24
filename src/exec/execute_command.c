/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 14:20:54 by acoronad          #+#    #+#             */
/*   Updated: 2025/10/24 03:21:42 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "exec.h"
#include "ast.h"
#include "builtins.h"

/*
// --- helper local ---
static int is_all_n(const char *s)
{
	(void)s;
	
	return (0);
}
*/

static void set_child_signals_default(void)
{
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
}

void restore_std_fds(int original_stdin, int original_stdout, int original_stderr)
{
    if (original_stdin != -1)  { dup2(original_stdin, STDIN_FILENO);  close(original_stdin);  }
    if (original_stdout != -1) { dup2(original_stdout, STDOUT_FILENO); close(original_stdout); }
    if (original_stderr != -1) { dup2(original_stderr, STDERR_FILENO); close(original_stderr); }
}

int execute_command(t_ast *node, t_shell *shell)
{
    pid_t   pid;
    char    **env;
    char    *exec_path;
    int     original_stdin = -1, original_stdout = -1, original_stderr = -1;

    if (!node || !node->cmd.argv || !node->cmd.argv[0])
        return (shell->exit_status = 0);

    if (node->cmd.argv[0][0] == '\0')
    {
        ft_dprintf(2, "minishell: : command not found\n");
        return (shell->exit_status = 127);
    }

    // *** CASO: ejecutamos dentro de un HIJO ya forkeado (pipes, subshell, background)
    if (shell->in_child)
    {
        // El hijo debe tener señales por defecto
        set_child_signals_default();

        // Redirecciones se aplican AQUÍ (en el propio hijo)
        if (apply_redirections(node->cmd.redirections) != 0)
            _exit(1);

        // Builtin en hijo: ejecutar y salir
        if (is_builtin(node->cmd.argv[0]))
        {
            int st = run_builtin(node, shell);
            _exit(st);
        }

        // Externo en hijo: resolver y execve
        exec_path = find_executable(node->cmd.argv[0], shell);
        if (!exec_path)
            _exit(print_exec_error(shell, node->cmd.argv[0], ENOENT));

        env = env_to_array(shell->env);
        if (!env)
        {
            free(exec_path);
            _exit(1);
        }

        execve(exec_path, node->cmd.argv, env);
        // Si llegamos aquí, execve falló
        int err = errno;
        free(exec_path);
        // env no se libera estrictamente necesario (salimos ya)
        _exit(print_exec_error(shell, node->cmd.argv[0], err));
    }

    // *** CASO: proceso PADRE (no forkeado todavía) ***
    // Builtins en el padre: aplicar redirecciones temporalmente
    if (is_builtin(node->cmd.argv[0]))
    {
        original_stdin  = dup(STDIN_FILENO);
        original_stdout = dup(STDOUT_FILENO);
        original_stderr = dup(STDERR_FILENO);
        if (original_stdin == -1 || original_stdout == -1 || original_stderr == -1)
        {
            perror("minishell: dup");
            if (original_stdin  != -1) close(original_stdin);
            if (original_stdout != -1) close(original_stdout);
            if (original_stderr != -1) close(original_stderr);
            return (shell->exit_status = 1);
        }
        if (apply_redirections(node->cmd.redirections) != 0)
        {
            restore_std_fds(original_stdin, original_stdout, original_stderr);
            return (shell->exit_status = 1);
        }
        shell->exit_status = run_builtin(node, shell);
        restore_std_fds(original_stdin, original_stdout, original_stderr);
        return shell->exit_status;
    }

    // Externo: fork y exec en el HIJO
    exec_path = find_executable(node->cmd.argv[0], shell);
    if (!exec_path)
        return print_exec_error(shell, node->cmd.argv[0], ENOENT);

    env = env_to_array(shell->env);
    if (!env)
    {
        free(exec_path);
        return (shell->exit_status = 1);
    }

    pid = fork();
    if (pid < 0)
    {
        free(exec_path);
        ft_free_strtab(env);
        return print_exec_error(shell, node->cmd.argv[0], errno);
    }
    if (pid == 0) // hijo real
    {
        set_child_signals_default();

        // Redirecciones SOLO en el hijo para externos
        if (apply_redirections(node->cmd.redirections) != 0)
            _exit(1);

        execve(exec_path, node->cmd.argv, env);
        // fallo
        int err = errno;
        _exit(print_exec_error(shell, node->cmd.argv[0], err));
    }

    // padre espera al HIJO y traduce status
    int st = 0;
    waitpid(pid, &st, 0);
    if (WIFEXITED(st))
        shell->exit_status = WEXITSTATUS(st);
    else if (WIFSIGNALED(st))
        shell->exit_status = 128 + WTERMSIG(st);
    else
        shell->exit_status = 1;

    free(exec_path);
    ft_free_strtab(env);
    return shell->exit_status;
}
