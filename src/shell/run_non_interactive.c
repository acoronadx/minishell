/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_non_interactive.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 20:32:53 by acoronad          #+#    #+#             */
/*   Updated: 2025/11/06 12:28:43 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Quita '\n' final si existe (opcionalmente también '\r' si llegara CRLF).
// Compatibilidad CRLF: si el script viene de Windows, acabará en \r\n.
// Sin el strip de \r, el último token podría quedar con un \r colgando
// (p. ej., "echo\r")

static void	strip_nl(char *s)
{
	size_t	len;

	if (!s)
		return ;
	len = ft_strlen(s);
	if (len && s[len - 1] == '\n')
	{
		s[len - 1] = '\0';
		if (len >= 2 && s[len - 2] == '\r')
			s[len - 2] = '\0';
	}
}

void	run_non_interactive(t_shell *shell)
{
	char	*line;

	shell->is_interactive = 0;
	setup_default_signals();
	while (1)
	{
		cleanup_loop(shell); /* limpia line/tokens previos */
		line = get_next_line(STDIN_FILENO);
		if (!line)
			break ;
		strip_nl(line);
		shell->line = line;
		if (*shell->line)
		{
			setup_exec_parent_signals();
			shell_exec(shell);
		}
		if (shell->should_exit)
			break ;
	}
	/* no imprimir "exit\n" en no-interactivo */
}
