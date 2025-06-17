/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_heredocs.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 05:58:04 by acoronad          #+#    #+#             */
/*   Updated: 2025/06/17 07:54:26 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	open_heredoc_pipe(int *pipe_fd)
{
	if (pipe(pipe_fd) == -1)
		return (1);
	return (0);
}

static int	read_heredoc_lines(const char *delim, int write_fd)
{
	char	*line;

	while (1)
	{
		ft_putstr_fd("> ", 1);
		line = get_next_line(0);
		if (!line)
			break;
		if (ft_strcmp(line, (char *)delim) == 0)
		{
			free(line);
			break;
		}
		ft_putstr_fd(line, write_fd);
		free(line);
	}
	return (0);
}

int	handle_heredoc(const char *delim)
{
	int	pipe_fd[2];

	if (open_heredoc_pipe(pipe_fd))
		return (-1);
	read_heredoc_lines(delim, pipe_fd[1]);
	close(pipe_fd[1]);
	return (pipe_fd[0]);
}
