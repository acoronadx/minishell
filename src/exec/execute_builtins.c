/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtins.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 06:02:29 by acoronad          #+#    #+#             */
/*   Updated: 2025/06/17 07:49:54 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strcmp(cmd, "echo") == 0)
		return (1);
	if (ft_strcmp(cmd, "cd") == 0)
		return (1);
	if (ft_strcmp(cmd, "pwd") == 0)
		return (1);
	if (ft_strcmp(cmd, "export") == 0)
		return (1);
	if (ft_strcmp(cmd, "unset") == 0)
		return (1);
	if (ft_strcmp(cmd, "env") == 0)
		return (1);
	if (ft_strcmp(cmd, "exit") == 0)
		return (1);
	return (0);
}
/*
int	execute_builtin(char **argv, t_shell *shell)
{
	if (ft_strcmp(argv[0], "echo") == 0)
		return (builtin_echo(argv, shell));
	if (ft_strcmp(argv[0], "cd") == 0)
		return (builtin_cd(argv, shell));
	if (ft_strcmp(argv[0], "pwd") == 0)
		return (builtin_pwd(argv, shell));
	if (ft_strcmp(argv[0], "export") == 0)
		return (builtin_export(argv, shell));
	if (ft_strcmp(argv[0], "unset") == 0)
		return (builtin_unset(argv, shell));
	if (ft_strcmp(argv[0], "env") == 0)
		return (builtin_env(argv, shell));
	if (ft_strcmp(argv[0], "exit") == 0)
		return (builtin_exit(argv, shell));
	return (1);
}
*/