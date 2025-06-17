/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 12:46:24 by acoronad          #+#    #+#             */
/*   Updated: 2025/06/17 05:02:37 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;
	int		ret;

	setup_signals();
	ret = handle_help_version(argc, argv);
	if (ret != -1)
		return (ret);
	init_shell(&shell, envp);
	if (argc == 3 && ft_strcmp(argv[1], "-c") == 0)
		execute_one_command(argv[2], &shell);
	else if (argc == 2 && argv[1][0] != '-')
		execute_script(argv[1], &shell);
	else if (argc == 1 && isatty(0))
		run_interactive(&shell);
	else if (argc > 1)
	{
		print_usage(argv[1]);
		cleanup_shell(shell);
		return (2);
	}
	cleanup_shell(shell);
	return (shell.exit_status);
}


/************************************************************
** Minishell - Main function / Función principal
**
** ENGLISH:
** - Sets up signal handling at the start.
** - If called with 3 arguments and the first is "-c", executes a single 
**   command (like 'bash -c <command>').
** - If called with 2 arguments and the first is not an option, executes a 
**   script file (like 'bash <script.sh>').
** - If called with "--help", prints usage help and exits (like 'bash --help').
** - If called with "--version", prints version info and exits (like 'bash --version').
** - If called with no arguments and in an interactive terminal, launches the 
**   interactive shell loop.
** - If called with invalid options or extra arguments, prints usage and exits 
**   with error code 2.
** - Returns 0 on normal exit.
**
** - This structure mimics the standard behaviors and entry points of bash, 
**   providing compatibility and intuitive usage for users.
**
** ESPAÑOL:
** - Configura el manejo de señales al inicio.
** - Si se llama con 3 argumentos y el primero es "-c", ejecuta un solo 
**   comando (como 'bash -c <comando>').
** - Si se llama con 2 argumentos y el primero no es opción, ejecuta un script
**   (como 'bash <script.sh>').
** - Si se llama con "--help", muestra la ayuda de uso y sale (como 'bash --help').
** - Si se llama con "--version", muestra la versión y sale (como 'bash --version').
** - Si se llama sin argumentos y en terminal interactivo, lanza el bucle 
**   interactivo de la shell.
** - Si se llama con opciones inválidas o argumentos extra, muestra uso y sale 
**   con código de error 2.
** - Devuelve 0 en salida normal.
**
** - Esta estructura imita los comportamientos y puntos de entrada estándar 
**   de bash, proporcionando compatibilidad y un uso intuitivo para el usuario.
************************************************************/