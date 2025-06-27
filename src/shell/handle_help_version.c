/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_help_version.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 22:44:20 by acoronad          #+#    #+#             */
/*   Updated: 2025/06/26 15:20:48 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_help_version(int argc, char **argv)
{
	if (argc == 2 && ft_strcmp(argv[1], "--help") == 0)
	{
		print_help();
		return (0);
	}
	if (argc == 2 && ft_strcmp(argv[1], "--version") == 0)
	{
		print_version();
		return (0);
	}
	return (-1);
}

void	print_help(void)
{
	ft_dprintf(1, "minishell: a minimal shell implementation\n");
	ft_dprintf(1, "Usage:  ./minishell [option] [script-file]\n");
	ft_dprintf(1, "Options:\n");
	ft_dprintf(1, "        -c command      execute command\n");
	ft_dprintf(1, "        script-file     execute commands from script file\n");
	ft_dprintf(1, "        --help          display this help and exit\n");
	ft_dprintf(1, "        --version       output version information and exit\n");
	ft_dprintf(1, "\n");
	ft_dprintf(1, "If no option or script-file is given, minishell starts in interactive mode.\n");
}

void	print_usage(const char *invalid)
{
	if (invalid)
		ft_dprintf(2, "minishell: %s: invalid option\n", invalid);
	ft_dprintf(2, "Usage:  ./minishell [option] [script-file]\n");
	ft_dprintf(2, "Options:\n");
	ft_dprintf(2, "        -c command      execute command\n");
	ft_dprintf(2, "        script-file     execute commands from script file\n");
	ft_dprintf(2, "        --help          display this help and exit\n");
	ft_dprintf(2, "        --version       output version information and exit\n");
}

void	print_version(void)
{
	ft_dprintf(1, "minishell, version 1.0\n");
}
