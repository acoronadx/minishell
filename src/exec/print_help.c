/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_help.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 12:14:20 by acoronad          #+#    #+#             */
/*   Updated: 2025/06/11 12:14:22 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
