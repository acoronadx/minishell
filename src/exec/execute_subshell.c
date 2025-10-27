/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_subshell.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 14:13:14 by acoronad          #+#    #+#             */
/*   Updated: 2025/10/27 00:50:32 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int execute_subshell(t_ast *node, t_shell *shell)
{
    pid_t pid;
    int   st = 0;

    pid = fork();
    if (pid < 0)
    {
        ft_dprintf(2, "minishell: fork error\n");
        shell->exit_status = 1;
        return (1);
    }
    if (pid == 0)
    {
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);
        _exit(execute_ast(node->subshell.child, shell) & 0xFF);
    }
    if (waitpid(pid, &st, 0) < 0)
        shell->exit_status = 1;
    else if (WIFEXITED(st))
        shell->exit_status = WEXITSTATUS(st);
    else if (WIFSIGNALED(st))
    {
        int sig = WTERMSIG(st);
        if (sig == SIGINT)
            write(STDOUT_FILENO, "\n", 1);
        else if (sig == SIGQUIT)
            write(STDERR_FILENO, "Quit (core dumped)\n", 19);
        shell->exit_status = 128 + sig;
    }
    else
        shell->exit_status = 1;

    return (shell->exit_status);
}
