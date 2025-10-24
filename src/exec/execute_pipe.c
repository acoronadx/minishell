/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 14:11:56 by acoronad          #+#    #+#             */
/*   Updated: 2025/10/24 03:16:23 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"
#include "ast.h"

int     execute_pipe(t_ast *node, t_shell *shell)
{
        int     pipefd[2];
        pid_t   pid_left;
        pid_t   pid_right;
        int     status_left = 0;
        int     status_right = 0;

        if (pipe(pipefd) == -1)
        {
                ft_dprintf(2, "minishell: pipe error\n");
                shell->exit_status = 1;
                return (1);
        }

        pid_left = fork();
        if (pid_left == 0)
        {
                // hijo izquierdo: escribe en pipe
                signal(SIGINT, SIG_DFL);
                signal(SIGQUIT, SIG_DFL);
                shell->in_child = 1;

                close(pipefd[0]);
                if (dup2(pipefd[1], STDOUT_FILENO) < 0) _exit(1);
                close(pipefd[1]);

                int code = execute_ast(node->bin.left, shell);
                _exit(code);
        }

        pid_right = fork();
        if (pid_right == 0)
        {
                // hijo derecho: lee del pipe
                signal(SIGINT, SIG_DFL);
                signal(SIGQUIT, SIG_DFL);
                shell->in_child = 1;

                close(pipefd[1]);
                if (dup2(pipefd[0], STDIN_FILENO) < 0) _exit(1);
                close(pipefd[0]);

                int code = execute_ast(node->bin.right, shell);
                _exit(code);
        }

        close(pipefd[0]);
        close(pipefd[1]);

        // Esperar a ambos, pero el status del PIPE es el del DERECHO (como bash)
        waitpid(pid_left, &status_left, 0);
        waitpid(pid_right, &status_right, 0);

        if (WIFEXITED(status_right))
                shell->exit_status = WEXITSTATUS(status_right);
        else if (WIFSIGNALED(status_right))
                shell->exit_status = 128 + WTERMSIG(status_right);
        else
                shell->exit_status = 1;

        return shell->exit_status;
}
