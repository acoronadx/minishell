/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 14:44:03 by acoronad          #+#    #+#             */
/*   Updated: 2025/07/04 12:35:45 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "ast.h"
#include "exec.h"
#include "builtins.h"

int run_builtin(t_ast *node, t_shell *shell)
{
	char **argv;

	if (!node || !node->cmd.argv || !node->cmd.argv[0])
		return (1);
	argv = node->cmd.argv;
	if (ft_strcmp(argv[0], "echo") == 0)
		return (run_echo(argv));
	if (ft_strcmp(argv[0], "cd") == 0)
		return (run_cd(argv, shell));
	if (ft_strcmp(argv[0], "pwd") == 0)
		return (run_pwd());
	if (ft_strcmp(argv[0], "export") == 0)
		return (run_export(argv, shell));
	if (ft_strcmp(argv[0], "unset") == 0)
		return (run_unset(argv, shell));
	if (ft_strcmp(argv[0], "env") == 0)
		return (run_env(shell));
	if (ft_strcmp(argv[0], "exit") == 0)
		return (run_exit(argv, shell));
	return (1);
}
