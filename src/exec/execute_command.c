/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 14:20:54 by acoronad          #+#    #+#             */
/*   Updated: 2025/07/22 13:17:56 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "exec.h"
#include "ast.h"
#include "builtins.h"

int execute_command(t_ast *node, t_shell *shell)
{
    pid_t   pid;
    char    **env;
    char    *exec_path;
    int     ret;
    int     original_stdin;
    int     original_stdout;
    int     original_stderr;

    if (!node || !node->cmd.argv || !node->cmd.argv[0])
        return (0);

    if (node->cmd.argv[0][0] == '\0')
    {
        ft_dprintf(2, "minishell: : command not found\n");
        shell->exit_status = 127;
        return (127);
    }
    if (g_signal) // Global variable for signal handling
    {
        shell->exit_status = 130;
        g_signal = 0;
        return (130);
    }

    // --- PASO 1: GUARDAR LOS FDs ORIGINALES ---
    // Esto se hace SIEMPRE, ya que las redirecciones pueden afectar tanto a built-ins como a comandos externos.
    original_stdin = dup(STDIN_FILENO);
    original_stdout = dup(STDOUT_FILENO);
    original_stderr = dup(STDERR_FILENO);

    if (original_stdin == -1 || original_stdout == -1 || original_stderr == -1) {
        perror("minishell: dup");
        shell->exit_status = 1;
        return (1);
    }

    // --- PASO 2: APLICAR LAS REDIRECCIONES ---
    // Llama a apply_redirections en el proceso principal.
    // Esta función debería manejar todos los tipos de redirección (<, >, >>, <<).
    // Si apply_redirections falla (ej. archivo no existe para lectura), debería devolver un error.
    ret = apply_redirections(node->cmd.redirections);
    if (ret != 0)
    {
        // Si hay un error en las redirecciones, restauramos y salimos.
        restore_std_fds(original_stdin, original_stdout, original_stderr);
        return (1); // Exit with error
    }

    // --- PASO 3: EJECUCIÓN DEL COMANDO ---
    if (is_builtin(node->cmd.argv[0]))
    {
        // Si es un built-in, se ejecuta directamente en el proceso de la shell.
        // run_builtin ya debería usar los FDs redirigidos (ej. STDOUT_FILENO).
        shell->exit_status = run_builtin(node, shell);

        // --- PASO 4: RESTAURAR LOS FDs ORIGINALES ---
        // ¡MUY IMPORTANTE para built-ins!
        restore_std_fds(original_stdin, original_stdout, original_stderr);
        return (shell->exit_status);
    }
    else // Si no es un built-in, es un comando externo
    {
        exec_path = find_executable(node->cmd.argv[0], shell);
        if (!exec_path)
        {
            // Restaurar FDs antes de devolver error
            restore_std_fds(original_stdin, original_stdout, original_stderr);
            return (print_exec_error(shell, node->cmd.argv[0], errno));
        }
        env = env_to_array(shell->env);
        if (!env)
        {
            free(exec_path);
            restore_std_fds(original_stdin, original_stdout, original_stderr);
            return (1);
        }
        
        pid = fork();
        if (pid < 0)
        {
            free(exec_path);
            ft_free_strtab(env);
            restore_std_fds(original_stdin, original_stdout, original_stderr);
            return (print_exec_error(shell, node->cmd.argv[0], errno));
        }
        if (pid == 0) // Proceso hijo
        {
            // Para comandos externos, los FDs ya están redirigidos en el padre,
            // pero el hijo los hereda. Aún así, es buena práctica llamarlo aquí de nuevo
            // si la lógica de `apply_redirections` maneja cierre de FDs que no son std
            // o para asegurar que el hijo tiene los correctos antes de execve.
            // Ojo: Si apply_redirections abre FDs nuevos, deben cerrarse tras el dup2.
            
            // Si apply_redirections *abre* y *cierra* FDs, y sólo hace el dup2,
            // entonces no es necesario volver a llamarla aquí. Los FDs se heredan.
            // Si apply_redirections solo usa dup2, los FDs originales están cerrados.
            // La única razón para llamarlo de nuevo es si quieres que el hijo maneje
            // sus propios FDs temporales (como heredoc) que el padre no.
            
            // Para mantenerlo simple y seguro, es mejor que apply_redirections
            // solo realice dup2, y que los FDs de archivos abiertos para redirección
            // se cierren inmediatamente después de cada dup2.
            
            // Ejemplo: open("file", ...) -> new_fd; dup2(new_fd, STDOUT_FILENO); close(new_fd);
            // Esto asegura que solo STDIN/OUT/ERR estén abiertos y redirigidos.

            signal(SIGINT, SIG_DFL); // Restablecer manejadores de señal para el hijo
            signal(SIGQUIT, SIG_DFL);
            
            // No necesitas `apply_redirections` aquí de nuevo si ya se hizo en el padre
            // y los FDs se cierran inmediatamente después del dup2.
            // Sin embargo, si tu apply_redirections abre muchos FDs y los mantiene abiertos
            // (lo cual NO es recomendable), entonces podrías necesitar una forma
            // de cerrar FDs no estándar en el hijo antes de execve.

            execve(exec_path, node->cmd.argv, env);
            // Si execve falla, imprime error y sale del hijo
            print_exec_error(shell, node->cmd.argv[0], errno);
            exit(127);
        }
        // Proceso padre
        waitpid(pid, &shell->exit_status, 0);
        if (WIFEXITED(shell->exit_status))
            shell->exit_status = WEXITSTATUS(shell->exit_status);
        else if (WIFSIGNALED(shell->exit_status))
            shell->exit_status = 128 + WTERMSIG(shell->exit_status);
        
        free(exec_path);
        ft_free_strtab(env);

        // --- PASO 4: RESTAURAR LOS FDs ORIGINALES ---
        // ¡También importante para comandos externos!
        restore_std_fds(original_stdin, original_stdout, original_stderr);
    }
    return (shell->exit_status);
}

// --- NUEVA FUNCIÓN DE AYUDA ---
// Función para restaurar los descriptores de archivo estándar
void restore_std_fds(int original_stdin, int original_stdout, int original_stderr)
{
    // Restaurar STDIN, STDOUT, STDERR a sus valores originales.
    // Asegúrate de que los FDs originales no sean -1 antes de usarlos.
    if (original_stdin != -1)
    {
        dup2(original_stdin, STDIN_FILENO);
        close(original_stdin);
    }
    if (original_stdout != -1)
    {
        dup2(original_stdout, STDOUT_FILENO);
        close(original_stdout);
    }
    if (original_stderr != -1)
    {
        dup2(original_stderr, STDERR_FILENO);
        close(original_stderr);
    }
}
