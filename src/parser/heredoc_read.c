/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_read.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** heredoc_read:
** Lee un heredoc hasta el delimitador o Ctrl+C. Devuelve fd de lectura,
** o -1 en error/interrupción.
*/

int	heredoc_read(char *delimiter)
{
	int		pipefd[2];
	char	*line;

	if (!delimiter)
		return (-1);
	if (pipe(pipefd) < 0)
	{
		perror("minishell: pipe");
		return (-1);
	}
	signal(SIGQUIT, SIG_DFL);
	signal(SIGINT, SIG_DFL);
	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		write(pipefd[1], line, ft_strlen(line));
		write(pipefd[1], "\n", 1);
		free(line);
	}
	close(pipefd[1]);
	return (pipefd[0]);
}

/*
//heredoc_read.c
int	read_heredoc_body(int script_fd, const char *delim, int quoted, int *out_fd)
{
	char	*line;
	size_t	n;

	int   tmp = mkstemp(tmpname); // o pipe()
	if (tmp < 0) return (-1);
	while (1)
	{
		line = get_next_line(script_fd); // en
			-s y no-interactivo: del mismo flujo
		if (!line) return (close(tmp), -1); // EOF inesperado: error sintaxis
		// quitar '\n' final si hay
		n = ft_strlen(line);
		if (n && line[n-1]=='\n') line[n-1]='\0';
		// fin del heredoc si coincide EXACTO con delim
		if (!ft_strcmp(line, delim))
		{
			free(line);
			break ;
		}
		// escribir (posible expansión si !quoted)
		write(tmp, line, ft_strlen(line));
		write(tmp, "\n", 1);
		free(line);
	}
	lseek(tmp, 0, SEEK_SET);
	*out_fd = tmp;
	return (0);
}
*/