/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_heredocs.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 15:27:07 by acoronad          #+#    #+#             */
/*   Updated: 2025/11/04 16:15:48 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "minishell.h"

/*
** Heredoc: lee hasta 'delimiter', escribe en un pipe y devuelve fd de lectura.
** Se usa en tiempo de ejecución de la redirección.
*/
int	heredoc_prepare(const char *delimiter)
{
	int		pipefd[2];
	char	*line;
	size_t	dlen;

	if (!delimiter)
		return (-1);
	dlen = ft_strlen(delimiter);
	if (pipe(pipefd) == -1)
		return (-1);
	/* Mientras se lee el heredoc, ^C aborta lectura; ^\ se ignora */
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, SIG_DFL);
	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (ft_strncmp(line, delimiter, dlen) == 0 && line[dlen] == '\0')
		{
			free(line);
			break ;
		}
		write(pipefd[1], line, ft_strlen(line));
		write(pipefd[1], "\n", 1);
		free(line);
	}
	close(pipefd[1]);
	/* Restauramos defaults típicos del hijo al salir del heredoc */
	signal(SIGQUIT, SIG_DFL);
	signal(SIGINT, SIG_DFL);
	return (pipefd[0]);
}
