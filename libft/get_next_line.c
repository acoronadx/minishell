/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarodri2 <rarodri2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 13:00:56 by rarodri2          #+#    #+#             */
/*   Updated: 2025/01/30 13:00:56 by rarodri2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*read_from_fd(int fd, char *buffer, int *error)
{
	char	*temp;
	ssize_t	bytes_read;
	char	*new_buffer;

	temp = (char *)malloc(BUFFER_SIZE + 1);
	if (!temp)
		return (NULL);
	bytes_read = read(fd, temp, BUFFER_SIZE);
	if (bytes_read <= 0)
	{
		if (bytes_read == -1)
			*error = 1;
		return (free(temp), NULL);
	}
	temp[bytes_read] = '\0';
	if (!buffer)
		return (ft_strdup(temp));
	new_buffer = ft_strjoin(buffer, temp);
	free(buffer);
	free(temp);
	return (new_buffer);
}

static char	*extract_line(char **buffer)
{
	char	*line;
	char	*newline_pos;
	char	*new_buffer;

	if (!*buffer || !**buffer)
	{
		free(*buffer);
		*buffer = NULL;
		return (NULL);
	}
	newline_pos = ft_strchr(*buffer, '\n');
	if (newline_pos)
	{
		line = ft_substr(*buffer, 0, newline_pos - *buffer + 1);
		new_buffer = ft_strdup(newline_pos + 1);
		free(*buffer);
		*buffer = new_buffer;
	}
	else
	{
		line = ft_strdup(*buffer);
		free(*buffer);
		*buffer = NULL;
	}
	return (line);
}

char	*get_next_line(int fd)
{
	static char	*buffer;
	char		*line;
	char		*temp;
	int			error;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	error = 0;
	if (!buffer)
		buffer = ft_strdup("");
	while (buffer && !ft_strchr(buffer, '\n'))
	{
		temp = read_from_fd(fd, buffer, &error);
		if (!temp)
			break ;
		buffer = temp;
	}
	if (error == 1 || !buffer || !*buffer)
	{
		free(buffer);
		buffer = NULL;
		return (NULL);
	}
	line = extract_line(&buffer);
	return (line);
}
/* #include <stdio.h>

int	main(void)
{
	int		fd;
	char	*line;
	int		count;

	count = 0;
	fd = open("read_error.txt", O_RDONLY);
	if (fd == -1)
	{
		printf("OPENING FILE ERROR\n");
		return (1);
	}
	line = get_next_line(fd);
	while (line)
	{
		count++;
		printf("%s", line);
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	return (0);
} */
/* int	main(void)
{
	int		fd_1;
	int		i;
	char	*line[4096];

	i = 1;
	fd_1 = open("J. K. Rowling - Harry Potter 1 - Sorcerer's Stone.txt", \
					O_RDONLY);
	while (1)
	{
		line[fd_1] = get_next_line(fd_1);
		printf("Line %d for fd %d: %s\n", i, fd_1, line[fd_1]);
		if (!line[fd_1])
			return (0);
		free(line[fd_1]);
		i++;
	}
} */