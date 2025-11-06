/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_interactive.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 13:09:44 by acoronad          #+#    #+#             */
/*   Updated: 2025/11/05 13:46:25 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_blank(const char *s)
{
	int	i;

	if (!s)
		return (1);
	i = 0;
	while (s[i] && ft_isspace((unsigned char)s[i]))
		i++;
	return (s[i] == '\0');
}

void	run_interactive(t_shell *shell)
{
	shell->is_interactive = 1;
	while (1)
	{
		setup_prompt_signals();
		cleanup_loop(shell); /* limpia line/tokens previos */
		shell->line = read_line_interactive(shell);
		if (!shell->line) /* Ctrl-D en prompt vacío */
			break ;
		if (is_blank(shell->line)) /* ignora líneas vacías/espacios */
			continue ;
		add_history(shell->line);
		setup_exec_parent_signals();
		shell_exec(shell); /* cleanup se hace al inicio del bucle */
		if (shell->should_exit)
			break ;
	}
	if (shell->is_interactive) /* Bash imprime "exit" al salir de REPL */
		write(1, "exit\n", 5);
	rl_clear_history();
}
