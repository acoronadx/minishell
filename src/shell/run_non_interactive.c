/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_non_interactive.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 20:32:53 by acoronad          #+#    #+#             */
/*   Updated: 2025/10/26 16:46:27 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "lexer.h"
#include "parser.h"
#include "expand.h"
#include "exec.h"

// Quita '\n' final si existe (opcionalmente también '\r' si llegara CRLF).
static void chomp_nl(char *s)
{
    size_t len;

    if (!s)
        return;
    len = ft_strlen(s);
    if (len && s[len - 1] == '\n')
    {
        s[len - 1] = '\0';
        if (len >= 2 && s[len - 2] == '\r') // por si viniera CRLF
            s[len - 2] = '\0';
    }
}

void run_non_interactive(t_shell *shell)
{
    char *line;

    shell->is_interactive = 0;
    // En no-interactivo, el caller debe haber llamado a setup_default_signals()
    // para no redibujar prompt y dejar señales por defecto.
    while (1)
    {
        if (shell->line)
        {
            free(shell->line);
            shell->line = NULL;
        }
        line = get_next_line(STDIN_FILENO);   // <-- usamos tu GNL
        if (!line)
            break;                            // EOF o error -> salir bucle
        chomp_nl(line);                        // quitamos '\n' final
        shell->line = line;
        if (*shell->line)                      // ignorar líneas vacías
        {
            // El padre no debe imprimir nada ni reaccionar a SIGINT/SIGQUIT.
            // Los hijos restaurarán señales por defecto antes de execve().
            setup_exec_parent_signals();
            if (shell->tokens)
            {
                free_token_list(shell->tokens);
                shell->tokens = NULL;
            }
            shell_exec(shell);
        }
        cleanup_loop(shell);
        if (shell->should_exit)
            break;
    }
    // Importante: NO imprimir "exit\n" aquí (solo en modo interactivo).
}
