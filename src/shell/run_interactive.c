/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_interactive.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 13:09:44 by acoronad          #+#    #+#             */
/*   Updated: 2025/10/27 00:41:11 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* helpers de cierre de comillas/backslash (tus versiones) */
static int ft_quotes_closed(const char *line)
{
    int i = 0, state = 0;
    while (line && line[i])
    {
        if (!state && (line[i] == '\'' || line[i] == '"')) state = line[i];
        else if (state == '\'' && line[i] == '\'') state = 0;
        else if (state == '"' && line[i] == '\\' &&
                 (line[i + 1] == '"' || line[i + 1] == '\\' || line[i + 1] == '`')) i++;
        else if (state == '"' && line[i] == '"') state = 0;
        else if (!state && line[i] == '\\' && line[i + 1]) i++;
        i++;
    }
    return (state == 0);
}

static int ft_backslach_closed(char *line)
{
    int i, count;

    if (!line) return (1);
    i = (int)ft_strlen(line) - 1;
    count = 0;
    while (i >= 0 && line[i] == '\\') { count++; i--; }
    return ((count % 2) == 0);
}

char *read_line_interactive(t_shell *shell)
{
    char *next;

    /* aseguramos prompt limpio y TTY sano */
    setup_prompt_signals();
    g_signal = 0;

    shell->line = readline("\001\033[1;35m\002minishell$ \001\033[0m\002");
    if (!shell->line)
        return NULL;

    while (!ft_quotes_closed(shell->line) || !ft_backslach_closed(shell->line))
    {
        setup_prompt_signals();
        next = readline("> ");
        if (!next) { free(shell->line); shell->line = NULL; return NULL; }
        shell->line = ft_strjoin_free_s1(shell->line, "\n");
        if (!shell->line) { free(next); return NULL; }
        shell->line = ft_strjoin_free_s1(shell->line, next);
        free(next);
    }
    return shell->line;
}
void run_interactive(t_shell *shell)
{
    shell->is_interactive = 1;

    while (1)
    {
        setup_prompt_signals();          /* prompt: INT handler + QUIT ignorado */
        if (shell->line) { free(shell->line); shell->line = NULL; }

        shell->line = read_line_interactive(shell); /* usa readline con el prompt */

        if (!shell->line)                 /* Ctrl-D en línea vacía */
            break;

        if (shell->line[0] == '\0') {     /* Enter vacío o Ctrl-C en prompt */
            free(shell->line);
            shell->line = NULL;
            continue;
        }

        add_history(shell->line);

        /* PADRE ignora señales mientras ejecuta (y las restablece el hijo) */
        setup_exec_parent_signals();

        if (shell->tokens) { free_token_list(shell->tokens); shell->tokens = NULL; }
        shell_exec(shell);

        /* Al volver de ejecutar, vuelve al modo prompt */
        setup_prompt_signals();
        cleanup_loop(shell);

        if (shell->should_exit)
            break;
    }
    if (shell->is_interactive)
        write(1, "exit\n", 5);
    rl_clear_history();
}
