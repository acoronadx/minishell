/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_modes.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 12:52:57 by acoronad          #+#    #+#             */
/*   Updated: 2025/06/27 14:39:57 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "signals.h"
//#include "exec.h"

/*
static int	run_one_command_mode(char **argv, t_shell *shell)
{
	shell->is_one_command = 1;
	shell->is_interactive = 0;
	execute_one_command(argv[2], shell);
	return (shell->exit_status);
}

static int	run_script_mode(char **argv, t_shell *shell)
{
	shell->is_script = 1;
	shell->is_interactive = 0;
	execute_script(argv[1], shell);
	return (shell->exit_status);
}
*/
static int	run_interactive_mode(t_shell *shell)
{
	run_interactive(shell);
	return (shell->exit_status);
}
/*
static int	handle_invalid_args(char *program_name)
{
	print_usage(program_name);
	return (2);
}
*/

int	run_shell_modes(int argc, char **argv, char **envp, t_shell *shell)
{
	int	ret;

	ret = handle_help_version(argc, argv);
	if (ret != -1)
		return (ret);
	if (init_env(shell, envp) != 0)
		return (1);
	shell->is_interactive = isatty(STDIN_FILENO);
	//if (argc == 3 && ft_strcmp(argv[1], "-c") == 0)
	//	return (run_one_command_mode(argv, shell));
	//else if (argc == 2 && argv[1][0] != '-')
	//	return (run_script_mode(argv, shell));
	//else if (argc == 1)
	//{
		setup_signals();
		return (run_interactive_mode(shell));
	//}
	//else if (argc > 1)
	//	return (handle_invalid_args(argv[0]));

	// Si llegamos aquí, es un error de uso
	print_usage(shell->program_name);
	return (2);
}
