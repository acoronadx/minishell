/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_loop.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 22:17:06 by acoronad          #+#    #+#             */
/*   Updated: 2025/11/07 22:40:11 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	write_nl(int fd, char *s)
{
	int	len;
	int	wr;

	len = (int)ft_strlen(s);
	wr = (int)write(fd, s, len);
	if (wr < 0)
		return (-1);
	wr = (int)write(fd, "\n", 1);
	if (wr < 0)
		return (-1);
	return (0);
}

/* ≤25 líneas, ≤5 vars, solo while */
int	hd_loop(t_shell *sh, int fdw, const char *delim, int quoted)
{
	char	*line;
	char	*tmp;

	line = NULL;
	tmp = NULL;
	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (ft_strcmp(line, delim) == 0)
		{
			free(line);
			break ;
		}
		if (!quoted)
		{
			tmp = expand_heredoc_line(sh, line);
			free(line);
			line = tmp;
			if (!line)
				return (-1);
		}
		if (write_nl(fdw, line) < 0)
			return (free(line), -1);
		free(line);
	}
	return (0);
}

