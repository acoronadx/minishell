/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_script.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 12:22:19 by acoronad          #+#    #+#             */
/*   Updated: 2025/06/17 04:57:35 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void clean_line(char **line)
{
	if (*line)
	{
		free(*line);
		*line = NULL;
	}
}

void	execute_script(const char *filename, t_shell *shell)
{
	int		fd;
	char	*line;

	shell->is_script = 1;
	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		ft_dprintf(2, "minishell: cannot open script file: %s\n", filename);
		return ;
	}
	line = get_next_line(fd);
	while (line)
	{
		if (shell->line)
			clean_line(&shell->line);
		shell->line = line;
		parse_and_execute(shell);
		cleanup_loop(shell);
		line = get_next_line(fd);
	}
	close(fd);
}
