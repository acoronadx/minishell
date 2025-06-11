/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_one_command.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 13:20:33 by acoronad          #+#    #+#             */
/*   Updated: 2025/06/11 13:24:56 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Ejecuta un solo comando, como en 'bash -c "comando"'.
** command_str: la cadena con el comando a ejecutar
** envp: el entorno de la minishell
*/
void	execute_one_command(char *command_str, char **envp)
{
	char	*line;

	line = NULL;
	if (!command_str)
		return ;
	line = ft_strdup(command_str);
	if (!line)
		return ;
	parse_and_execute(line, envp);
	free(line);
}
