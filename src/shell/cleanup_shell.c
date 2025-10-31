/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_shell.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 03:25:37 by acoronad          #+#    #+#             */
/*   Updated: 2025/10/31 22:12:22 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "lexer.h"
#include "env.h"
#include <string.h>
#include <errno.h>

/* --- NUEVO: helper sin 'for' para Norma --- */
static int	has_slash_local(const char *s)
{
	int	i;

	if (!s)
		return (0);
	i = 0;
	while (s[i])
	{
		if (s[i] == '/')
			return (1);
		i++;
	}
	return (0);
}

int	print_exec_error(t_shell *shell, const char *cmd, int err_code)
{
	int	ret;

	if (!cmd || !*cmd)
	{
		ft_dprintf(2, "minishell: : command not found\n");
		shell->exit_status = 127;
		return (127);
	}
	if (!has_slash_local(cmd) && err_code == ENOENT)
	{
		ft_dprintf(2, "minishell: %s: command not found\n", cmd);
		ret = 127;
	}
	else
	{
		ft_dprintf(2, "minishell: %s: %s\n", cmd, strerror(err_code));
		if (err_code == EACCES || err_code == ENOEXEC
			|| err_code == EISDIR || err_code == EPERM)
			ret = 126;
		else if (err_code == ENOENT || err_code == ENOTDIR
			|| err_code == ENAMETOOLONG || err_code == ELOOP)
			ret = 127;
		else
			ret = 126;
	}
	shell->exit_status = ret;
	return (ret);
}

void	cleanup_loop(t_shell *shell)
{
	if (shell->line)
	{
		free(shell->line);
		shell->line = NULL;
	}
	if (shell->tokens)
	{
		free_token_list(shell->tokens);
		shell->tokens = NULL;
	}
}

void	cleanup_shell(t_shell *shell)
{
	if (shell->env)
	{
		free_env_list(shell->env);
		shell->env = NULL;
	}
	if (shell->line)
	{
		free(shell->line);
		shell->line = NULL;
	}
	if (shell->tokens)
	{
		free_token_list(shell->tokens);
		shell->tokens = NULL;
	}
	if (shell->program_name)
	{
		free(shell->program_name);
		shell->program_name = NULL;
	}
	restore_vquit();
}
