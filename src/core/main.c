/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 19:32:28 by codespace         #+#    #+#             */
/*   Updated: 2025/06/06 19:42:04 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_quotes_closed(const char *line)
{
	int	single = 0;
	int	double_ = 0;

	while (*line)
	{
		if (*line == '\'' && double_ % 2 == 0)
			single++;
		else if (*line == '"' && single % 2 == 0)
			double_++;
		line++;
	}
	return ((single % 2 == 0) && (double_ % 2 == 0));
}

static char	*read_full_line(void)
{
	char	*line;
	char	*next;
	char	*tmp;

	line = readline("\001\033[1;35m\002minishell$ \001\033[0m\002");
	if (!line)
		return (NULL);
	while (!ft_quotes_closed(line))
	{
		next = readline("> ");
		if (!next)
			break ;
		tmp = ft_strjoin(line, "\n");
		free(line);
		line = ft_strjoin(tmp, next);
		free(tmp);
		free(next);
	}
	return (line);
}

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	t_shell	shell;

	(void)argc;
	(void)argv;
	init_shell(&shell, envp);
	while (1)
	{
		line = read_full_line();
		if (!line)
		{
			perror("exit\n");
			break ;
		}
		if (*line)
			add_history(line);
		parse_and_execute(&shell, line);
//		free(line);
	}
	cleanup_shell(&shell);
	return (0); //¿?
}
