/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_background.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 14:19:18 by acoronad          #+#    #+#             */
/*   Updated: 2025/10/24 03:57:32 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"
#include "ast.h"

 int execute_background(t_ast *node, t_shell *shell)
 {
         pid_t   pid;
 
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
                _exit(execute_ast(node->bin.left, shell) & 0xFF);
         }
         // padre no espera al hijo en background
         shell->exit_status = 0;
         ft_dprintf(1,"[1] %d\n", pid);
         return (0);
 }
