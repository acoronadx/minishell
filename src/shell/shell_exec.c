/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 14:49:39 by acoronad          #+#    #+#             */
/*   Updated: 2025/06/11 16:13:34 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	parse_and_execute(char *line, char **envp)
{
	t_command	*cmd_list;

	cmd_list = parse_line(line);
	if (!cmd_list)
	{
		// Error de parseo, devuelve o muestra mensaje
		return ;
	}
	expand_variables(cmd_list, envp);
	if (!check_syntax(cmd_list))
	{
		// Error de sintaxis, libera y devuelve
		free_commands(cmd_list);
		return ;
	}
	execute_command(cmd_list, envp);
	free_commands(cmd_list);
}
