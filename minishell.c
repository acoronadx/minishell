/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 12:46:24 by acoronad          #+#    #+#             */
/*   Updated: 2025/06/10 13:43:32 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	setup_signals();
	if (argc == 3 && ft_strcmp(argv[1], "-c") == 0)
	{
		// Ejecutar el comando pasado en argv[2]
		execute_one_command(argv[2], envp);
	}
	else if (argc == 2)
	{
		// Ejecutar el script pasado en argv[1]
		execute_script(argv[1], envp);
	}
	else
	{
		if (isatty(0))
		// Modo interactivo: mostrar prompt y leer comandos en bucle
			run_interactive(envp);
	}
	return (0);
}
