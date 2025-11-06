/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_modes.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 12:52:57 by acoronad          #+#    #+#             */
/*   Updated: 2025/11/05 14:35:33 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* ejecuta -c "cmd" y sale (no interactivo) */
int	run_c_mode(t_shell *sh, const char *cmd)
{
	setup_default_signals();
	sh->is_interactive = 0;
	if (!cmd)
		return (2);
	if (sh->line)
	{
		free(sh->line);
		sh->line = NULL;
	}
	sh->line = ft_strdup(cmd);
	if (!sh->line)
		return (1);
	if (sh->tokens)
	{
		free_token_list(sh->tokens);
		sh->tokens = NULL;
	}
	shell_exec(sh);
	cleanup_loop(sh);
	return (sh->exit_status);
}

/* lee desde stdin (-s) hasta EOF (no interactivo) */
int	run_s_mode(t_shell *sh)
{
	setup_default_signals();
	sh->is_interactive = 0;
	run_non_interactive(sh);
	return (sh->exit_status);
}

/* entrypoint: interactivo / no-interactivo / -c / -s */
int	run_shell_modes(int argc, char **argv, char **envp, t_shell *shell)
{
	int		ret;
	t_opts	opt;

	ret = handle_help_version(argc, argv);
	if (ret != -1)
		return (ret);
	if (init_env(shell, envp) != 0)
		return (1);
	ret = parse_args_simple(argc, argv, &opt);
	if (ret != 0)
		return (ret);
	if (opt.cstr)
		return (run_c_mode(shell, opt.cstr));
	if (opt.flag_s)
		return (run_s_mode(shell));
	shell->is_interactive = (opt.flag_i || (isatty(STDIN_FILENO)
				&& isatty(STDERR_FILENO)));
	if (shell->is_interactive)
		run_interactive(shell);
	else
		run_non_interactive(shell);
	return (shell->exit_status);
}
