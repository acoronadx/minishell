/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_read.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 01:06:08 by acoronad          #+#    #+#             */
/*   Updated: 2025/06/30 18:35:26 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "signal.h"

/*
** heredoc_read:
** Lee un heredoc hasta el delimitador o interrupción por Ctrl+C (SIGINT).
** - Devuelve un fd de solo lectura si todo va bien.
** - Devuelve -1 si hubo interrupción o error.
*/
int	heredoc_read(char *delimiter)
{
	int		pipefd[2];
	char	*line;

	if (pipe(pipefd) == -1)
		return (-1);

	g_signal = 0;
	signal(SIGINT, handle_sigint); // Establecer manejo personalizado para Ctrl+C
	signal(SIGQUIT, SIG_IGN);      // Ignorar SIGQUIT (Ctrl+\)

	while (!g_signal)
	{
		line = readline("> ");
		if (!line)
		{
			ft_dprintf(2,
				"minishell: warning: here-document delimited by EOF (wanted `%s')\n",
				delimiter);
			break;
		}
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break;
		}
		write(pipefd[1], line, ft_strlen(line));
		write(pipefd[1], "\n", 1);
		free(line);
	}

	close(pipefd[1]); // Cerramos escritura: fin de input

	signal(SIGINT, SIG_DFL);  // Restaurar señales
	signal(SIGQUIT, SIG_DFL);

	if (g_signal)
	{
		// Usuario pulsó Ctrl+C
		write(1, "\n", 1);
		rl_replace_line("", 0);  // Limpiar línea actual
		rl_on_new_line();
		rl_redisplay();
		close(pipefd[0]);        // No queremos devolver un fd inválido
		return (-1);
	}
	return (pipefd[0]); // Devolvemos el fd de lectura del pipe
}
