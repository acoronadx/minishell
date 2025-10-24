/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_background.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 14:19:18 by acoronad          #+#    #+#             */
/*   Updated: 2025/10/24 03:16:53 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"
#include "ast.h"

int execute_background(t_ast *node, t_shell *shell)
{
        pid_t   pid = fork();
        if (pid < 0)
        {
                ft_dprintf(2, "minishell: fork error\n");
                return (shell->exit_status = 1);
        }
        if (pid == 0)
        {
                signal(SIGINT, SIG_DFL);
                signal(SIGQUIT, SIG_DFL);
                shell->in_child = 1;

                int code = execute_ast(node->bin.left, shell);
                _exit(code);
        }
        // Padre no espera en background; status de shell no cambia (bash deja $? tal cual)
        // Si no quieres imprimir [1] PID, comenta la línea siguiente:
        // ft_dprintf(1, "[1] %d\n", pid);
        return shell->exit_status;
}
