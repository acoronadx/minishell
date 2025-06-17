/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_one_command.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 13:20:33 by acoronad          #+#    #+#             */
/*   Updated: 2025/06/17 07:54:43 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"
#include "parser.h"

/*
** Ejecuta un solo comando, como en 'bash -c "comando"'.
** command_str: la cadena con el comando a ejecutar
** envp: el entorno de la minishell
*/
void	execute_one_command(char *command_str, t_shell *shell)
{
	char	*line;

	shell->is_one_command = 1;
	line = NULL;
	if (!command_str)
		return ;
	line = ft_strdup(command_str);
	if (!line)
		return ;
	if (shell->line)
		clean_line(&shell->line);
	shell->line = line;
	parse_and_execute(shell);
	cleanup_loop(shell);
}
