/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_shell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 03:25:09 by acoronad          #+#    #+#             */
/*   Updated: 2025/06/17 04:47:08 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_shell(t_shell *shell, char **envp)
{
	shell->env = init_env_list(envp);
	shell->line = NULL;
	shell->tokens = NULL;
	shell->ast = NULL;
	shell->exit_status = 0;
	shell->is_interactive = 0;
	shell->is_script = 0;
	shell->is_one_command = 0;
	shell->should_exit = 0;
}
