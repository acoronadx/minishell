/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_subshell.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 14:13:14 by acoronad          #+#    #+#             */
/*   Updated: 2025/10/24 03:16:37 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"
#include "ast.h"

int execute_subshell(t_ast *node, t_shell *shell)
{
        pid_t   pid;
        int     status = 0;

        pid = fork();
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

                int code = execute_ast(node->subshell.child, shell);
                _exit(code);
        }
        if (waitpid(pid, &status, 0) == -1)
                return (shell->exit_status = 1);

        if (WIFEXITED(status))
                shell->exit_status = WEXITSTATUS(status);
        else if (WIFSIGNALED(status))
                shell->exit_status = 128 + WTERMSIG(status);
        else
                shell->exit_status = 1;
        return shell->exit_status;
}
