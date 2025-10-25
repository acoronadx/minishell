/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 14:20:54 by acoronad          #+#    #+#             */
/*   Updated: 2025/10/24 04:30:08 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "exec.h"
#include "ast.h"
#include "builtins.h"

/* Restaurar STDIN/OUT/ERR */
void restore_std_fds(int saved_in, int saved_out, int saved_err)
{
    if (saved_in != -1)  { dup2(saved_in, STDIN_FILENO);  close(saved_in);  }
    if (saved_out != -1) { dup2(saved_out, STDOUT_FILENO); close(saved_out); }
    if (saved_err != -1) { dup2(saved_err, STDERR_FILENO); close(saved_err); }
}

int execute_command(t_ast *node, t_shell *shell)
{
    pid_t   pid;
    char    **env;
    char    *exec_path;
    int     status;

    if (!node || !node->cmd.argv || !node->cmd.argv[0])
        return (0);

    /* argv[0] vacío => “command not found” */
    if (node->cmd.argv[0][0] == '\0')
    {
        ft_dprintf(2, "minishell: : command not found\n");
        shell->exit_status = 127; /* bash: not found -> 127. */ /* Bash manpage. */
        return (127);
    }

    /* -------- BUILTINS: redirecciones SOLO en el PADRE -------- */
    if (is_builtin(node->cmd.argv[0]))
    {
        int saved_in  = dup(STDIN_FILENO);
        int saved_out = dup(STDOUT_FILENO);
        int saved_err = dup(STDERR_FILENO);

        if (saved_in == -1 || saved_out == -1 || saved_err == -1)
        {
            perror("minishell: dup");
            shell->exit_status = 1;
            if (saved_in  != -1) close(saved_in);
            if (saved_out != -1) close(saved_out);
            if (saved_err != -1) close(saved_err);
            return (1);
        }

        if (apply_redirections(node->cmd.redirections) != 0)
        {
            /* Si fallan redirecciones, restauramos y devolvemos error */
            restore_std_fds(saved_in, saved_out, saved_err);
            shell->exit_status = 1;
            return (1);
        }

        shell->exit_status = run_builtin(node, shell);
        restore_std_fds(saved_in, saved_out, saved_err);
        return (shell->exit_status);
    }

    /* -------- EXTERNOS: redirecciones SOLO en el HIJO -------- */
    exec_path = find_executable(node->cmd.argv[0], shell);
    if (!exec_path)
    {
        /* bash: 127 si no se encuentra el comando, 126 si no es ejecutable, etc. */
        return (print_exec_error(shell, node->cmd.argv[0], errno)); /* Bash EXIT STATUS doc. */
    }

    env = env_to_array(shell->env);
    if (!env)
    {
        free(exec_path);
        shell->exit_status = 1;
        return (1);
    }

    pid = fork();
    if (pid < 0)
    {
        int ret = print_exec_error(shell, node->cmd.argv[0], errno);
        free(exec_path);
        ft_free_strtab(env);
        return (ret);
    }
    if (pid == 0)
    {
        /* ----- HIJO ----- */
        /* Señales por defecto, como en un binario normal */
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);

        /* Redirecciones AQUÍ (solo afectan al hijo) */
        if (apply_redirections(node->cmd.redirections) != 0)
            _exit(1);

        execve(exec_path, node->cmd.argv, env);
        /* Si llegamos aquí, execve falló: imprimir error y salir.
           bash: “not found” -> 127, “no ejecutable/permiso” -> 126, etc. */
        print_exec_error(shell, node->cmd.argv[0], errno);
        _exit(127);
    }

    /* ----- PADRE ----- */
    if (waitpid(pid, &status, 0) == -1)
        shell->exit_status = 1;
    else if (WIFEXITED(status))
        shell->exit_status = WEXITSTATUS(status);   /* wait(2) macros. */
    else if (WIFSIGNALED(status))
        shell->exit_status = 128 + WTERMSIG(status);/* Bash: 128+signal. */

    free(exec_path);
    ft_free_strtab(env);
    return (shell->exit_status);
}
