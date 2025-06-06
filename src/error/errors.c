/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 20:45:41 by codespace         #+#    #+#             */
/*   Updated: 2025/06/06 20:46:02 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void ft_print_exit_init_error(char *msg, t_shell *shell)
{
	perror(msg);
	cleanup_shell(shell);
	exit(1);
}

void	ft_print_exit_user_error(t_shell *shell, const char *msg, int code)
{
	ft_dprintf(1, "\033[31mError:\n\033[0m<<%s>>\n", msg);
	shell->exit_status = code; // Guarda el código de salida
	exit(code);
}

void	ft_print_exit_sys_error(t_shell *shell, const char *msg, int code)
{
	if (msg)
		ft_dprintf(2, "minishell: %s: %s\n", msg, strerror(errno));
	else
		perror("minishell");
	shell->exit_status = code;
	exit(code);
}
