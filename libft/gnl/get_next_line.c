/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 18:08:46 by acoronad          #+#    #+#             */
/*   Updated: 2025/02/07 14:17:38 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*ft_inicialize(char *rmnt)
{
	free(rmnt);
	return (NULL);
}

char	*ft_create_line(char *buffer, ssize_t bytes_read)
{
	static char	*rmnt;
	char		*line;
	char		*tmp;

	line = NULL;
	if (bytes_read == 0 && rmnt && (rmnt[ft_position(rmnt, '\n')] != '\n'))
	{
		line = rmnt;
		rmnt = NULL;
		return (line);
	}
	tmp = ft_strjoin(rmnt, buffer);
	free(rmnt);
	rmnt = tmp;
	if (rmnt[ft_position(rmnt, '\n')] == '\n')
	{
		line = ft_strldup(rmnt, ft_position(rmnt, '\n') + 1);
		tmp = ft_strldup(&rmnt[ft_position(rmnt, '\n') + 1],
				ft_strlen(&rmnt[ft_position(rmnt, '\n') + 1]));
		free(rmnt);
		rmnt = tmp;
	}
	if (rmnt[0] == '\0' || bytes_read < 0)
		rmnt = ft_inicialize(rmnt);
	return (line);
}

char	*get_next_line(int fd)
{
	char	*buffer;
	char	*line;
	ssize_t	bytes_read;

	line = NULL;
	bytes_read = 1;
	buffer = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buffer)
		return (NULL);
	buffer[BUFFER_SIZE] = '\0';
	while (!line && bytes_read > 0)
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read > -1)
			buffer[bytes_read] = '\0';
		line = ft_create_line(buffer, bytes_read);
	}
	if (buffer)
		free(buffer);
	return (line);
}
