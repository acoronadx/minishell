/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_script.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 12:22:19 by acoronad          #+#    #+#             */
/*   Updated: 2025/06/11 13:07:37 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_script(const char *filename, char **envp)
{
	int		fd;
	char	*line;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		ft_dprintf(2, "minishell: cannot open script file: %s\n", filename);
		return ;
	}
	line = get_next_line(fd);
	while (line)
	{
		parse_and_execute(line, envp);
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
}
